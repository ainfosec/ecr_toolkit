#!/bin/bash

# ** Ensure that we are running as root **
if ! [ $(id -u) = 0 ]; then
    echo "Run this script as root"
    exit 1
fi

standard_instructions=(XOR
                       CPUID
                       RDTSC
                       RDTSCP
                       INVD
                       CLTS
                       INB
                       OUTB
                       INVLPG
                       INVPCID
                       MOV_FROM_CR8
                       MOV_TO_CR0
                       MOV_TO_CR3
                       MOV_TO_CR4
                       MOV_TO_CR8
                       MOV_DR0
                       MOV_DR1
                       MOV_DR2
                       MOV_DR3
                       MOV_DR4
                       MOV_DR5
                       MOV_DR6
                       MOV_DR7
                       RDMSR
                       RDPMC
                       RDRAND
                       RDSEED
                       WBINVD
                       WRMSR
                       MOVNTDQA
                       MOVNTDQ)

flush_instructions=(CLFLUSH
                    SFENCE
                    MFENCE)

# ** Timing measurement functions **
function wall_parse {
    dm=$(dmesg | grep -A100000000 `dmesg | grep Inserting | tail -1 \
         | awk -F '[][]' '{print $2}'` | grep $1 | tail -1)
    parsed=${dm##*: }
    echo $parsed
}

function nop_parse {
    dm=$(dmesg | grep -A100000000 `dmesg | grep Inserting | tail -1 \
         | awk -F '[][]' '{print $2}'` | grep $1 -A 1 | tail -1)
    parsed=${dm##* }
    echo $parsed
}

function cache_parse {
    val=$1
    dm=$(dmesg | grep -A100000000 `dmesg | grep Inserting | tail -1 \
        | awk -F '[][]' '{print $2}'` | grep "Average clock cycles" | grep -w $val)
    parsed=${dm##* }
    echo $parsed
}

function memory_parse {
    val=$1
    dm=$(dmesg | grep -A100000000 `dmesg | grep Inserting | tail -1 \
        | awk -F '[][]' '{print $2}'` | grep "access page $val")
    parsed=${dm##* }
    echo $parsed
}

function average {
    temp=("$@")
    avg=0 ; count=0
    for i in "${temp[@]}"; do
        avg=$((avg + i))
        count=$((count + 1))
    done
    if [ "$count" -eq "0" ] ; then
        count=1
    fi
    avg=$((avg / count))
    echo $avg
}

function variance {
    avg=$1
    arr=("${@:2}")
    var=0 ; count=0
    for i in "${arr[@]}"; do
        count=$((count + 1))
        calc=$((i - avg))
        calc=$((calc * calc))
        var=$((var + calc))
    done
    if [ "$count" -eq "0" ] ; then
        count=1
    fi
    var=$((var / count))
    echo $var
}

function evaluate_exiting {
    temp1=$1
    temp2=$2
    wall_high=$((wall_avg_CPUID - temp1))
    wall_low=$((temp1 - wall_avg_XOR))
    nop_high=$((nop_avg_CPUID - temp2))
    nop_low=$((temp2 - nop_avg_XOR))
    exiting=""
    if [[ $wall_high -lt $wall_low ]] && [[ $nop_high -lt $nop_low ]] ; then
        exiting="X"
    elif [[ $wall_high -lt $wall_low ]] || [[ $nop_high -lt $nop_low ]] ; then
        exiting="?"
    fi
    echo $exiting
}

# ** Trap handling functions **
function ctrl_c() {
    printf "Ctrl+C received, exiting and cleaning up...\n" $1
    sudo rmmod $1
    exit 0
}

function finish() {
    sudo rmmod $1 2>/dev/null
    exit 0
}

# ** Command line argument parsing **
# This will use the GNU Enhanced getopt if available, and will otherwise fall
# back on the original getopt
program_name=`basename $0`

echo "$program_name: Starting..."

# Trap on errors, interrupts, and exits
trap "echo $program_name: error encountered: aborted" ERR
trap "ctrl_c $1" INT
trap "finish $1" EXIT

# Define options
opts_short=kcflmh
opts_long=kmalloc,csv,flush,cache,memory,help

help_short="Usage: $program_name [options] <kernel module>
When none of the below options are used, core instruction timing is run by default.
Options:
  -k       allocate kernel memory (required for non-temporal instructions)
  -c       output in csv file
  -f       run buffer flush instructions (will not run standard instruction timing)
  -l       run cache timing operations
  -m       run memory timing operations
  -h       show this help message"

help_long="Usage: $program_name [options] <kernel module>
When none of the below options are used, core instruction timing is run by default.
Options:
  -k, --kmalloc      allocate kernel memory (required for non-temporal instructions)
  -c, --csv          output in csv file
  -f, --flush        run buffer flush instructions (will not run standard instruction timing)
  -l, --cache        run cache timing operations
  -m, --memory       run memory timing operations
  -h, --help         show this help message"

# Check if GNU Enhanced getopt is available
enhanced_getopt_available=
if getopt -T >/dev/null; then :
else
    if [ $? -eq 4 ]; then
        enhanced_getopt_available=yes
    fi
fi

# Run getopt (running it in an if-statement so the trap does not interfere)
if [ -n "$enhanced_getopt_available" ]; then
    # Use GNU enhanced getopt
    if ! getopt --name "$program_name" --long $opts_long --options $opts_short -- "$@" >/dev/null; then
        echo "$program_name: error (use -h or --help for help)" >&2
        exit 2
    fi
    args=`getopt --name "$program_name" --long $opts_long --options $opts_short -- "$@"`
else
    # Use original getopt
    if ! getopt $opts_short "$@" >/dev/null; then
        echo "$program_name: error (use -h for help)" >&2
        exit 2
    fi
    args=`getopt $opts_short "$@"`
fi
eval set -- $args

# Process parsed options
while [ $# -gt 0 ]; do
    case "$1" in
        -k | --kmalloc)  kmalloc_enable=yes;;
        -c | --csv)      csv_enable=yes;;
        -f | --flush)    flush_enable=yes;
                         kmalloc_enable=yes;;
        -l | --cache)    cache_enable=yes;
                         kmalloc_enable=yes;;
        -m | --memory)   memory_enable=yes;
                         kmalloc_enable=yes;;
        -h | --help)     if [ -n "$enhanced_getopt_available" ]
                         then echo "$help_long";
                         else echo "$help_short";
                         fi;  exit 0;;
        --)              shift; break;; # end of options
    esac
    shift
done

sudo insmod $1
if [ $? -eq 1 ]; then
    echo "$program_name: Invalid kernel object."
    exit 1
fi

# Allocate memory, if necessary
if [ -n "$kmalloc_enable" ]; then
    echo "$program_name: Allocating memory..."
    sudo ./mod_ioctl 7 >/dev/null
    dm=$(dmesg | grep "Allocating memory" -A 1 | grep -o "0x[a-fA-F0-9]*" | tail -1)
    if [ -z "$dm" ]; then
        echo "$program_name: Memory allocation failed. Rerun the script."
        exit 1
    fi
    echo "$program_name: Memory has been allocated at kernel virtual address: $dm"
    read -p "$program_name: Press Enter to continue"
fi

# If running the buffer flushing instructions, set that as the array, and set the
# appropriate option numbers for the ioctl to use
if [ -n "$flush_enable" ]; then
    eval $(typeset -A -p flush_instructions|sed 's/ flush_instructions=/ instructions=/')
    ioctl_option_1="5"
    ioctl_option_2="6"
else
    eval $(typeset -A -p standard_instructions|sed 's/ standard_instructions=/ instructions=/')
    ioctl_option_1="1"
    ioctl_option_2="2"
fi
declare -a instructions

# ** Start of measurements **
if [ -n "$cache_enable" ]; then
    echo "$program_name: Running cache timing operations..."
    sudo ./mod_ioctl 3 >/dev/null
    echo ""
    echo "     Offset | Tick Count"
    echo "     -------|-----------"
    declare -a address_offsets=("0x0" "0x40" "0x80" "0x100" "0x1000")
    for i in "${address_offsets[@]}"
    do
        cache_arr[$i]=$(cache_parse $i)
        if [ -n "$csv_enable" ]; then
            # ** Print results to csv file **
            printf "%s,%s\n" $i ${cache_arr[$i]} >> results.csv
        else
            # ** Print results table **
            printf "    %7s |%11s\n" $i ${cache_arr[$i]}
        fi
    done
    sudo rmmod $1
elif [ -n "$memory_enable" ]; then
    echo "$program_name: Running memory timing operations..."
    sudo ./mod_ioctl 4 >/dev/null
    echo ""
    echo "     Offset | Tick Count"
    echo "     -------|-----------"
    for i in {0..9}
    do
        memory_arr[$i]=$(memory_parse $i)
        if [ -n "$csv_enable" ]; then
            # ** Print results to csv file **
            printf "%s,%s\n" $i ${memory_arr[$i]} >> results.csv
        else
            # ** Print results table **
            printf "    %7s |%11s\n" $i ${memory_arr[$i]}
        fi
    done
    sudo rmmod $1
else
    echo "$program_name: Running timing..."
    for i in `seq 1 15` ; do
        sudo ./mod_ioctl $ioctl_option_1 >/dev/null
        for j in "${instructions[@]}"; do
            eval name="WALL_$j:"
            eval wall_arr_$j[$i]=$(wall_parse $name)
        done

        sudo ./mod_ioctl $ioctl_option_2 >/dev/null
        for j in "${instructions[@]}"; do
            eval name="NOP_$j:"
            eval nop_arr_$j[$i]=$(nop_parse $name)
        done
    done
    sudo rmmod $1

    for j in "${instructions[@]}"; do
        eval wall_arr=\${wall_arr_$j[@]}
        eval nop_arr=\${nop_arr_$j[@]}
        eval wall_avg_$j=$(average ${wall_arr[@]})
        eval nop_avg_$j=$(average ${nop_arr[@]})
        eval wall_avg=\$wall_avg_$j
        eval nop_avg=\$nop_avg_$j
        eval wall_var_$j=$(variance $wall_avg ${wall_arr[@]})
        eval nop_var_$j=$(variance $nop_avg ${nop_arr[@]})
    done

    for j in "${instructions[@]}"; do
        eval wall_avg=\$wall_avg_$j
        eval nop_avg=\$nop_avg_$j
        eval exit_flag_$j=$(evaluate_exiting $wall_avg $nop_avg)
    done
    exit_flag_INB="-"
    exit_flag_OUTB="-"

    echo "$program_name: Finished"

    if [ -n "$csv_enable" ]; then
        # ** Print results to csv file **
        if [ -f ./results.csv ] ; then
            rm results.csv
        fi
        for j in "${instructions[@]}"; do
            eval name=\$j
            eval wall_avg=\$wall_avg_$j
            eval nop_avg=\$nop_avg_$j
            eval wall_var=\$wall_var_$j
            eval nop_var=\$nop_var_$j
            printf "%s,%d,%d,%d,%d\n" $name $wall_avg $nop_avg $wall_var $nop_var >> results.csv
        done
    else
        # ** Print results table **
        echo ""
        echo "     Instruction | Wall Time Avg | NOP Time Avg | Probable Exit"
        echo "     ------------|---------------|--------------|--------------"
        for j in "${instructions[@]}"; do
            eval name=\$j
            eval wall_avg=\$wall_avg_$j
            eval nop_avg=\$nop_avg_$j
            eval exit_flag=\$exit_flag_$j
            printf "    %12s |%14d |%13d |%14s\n" $name $wall_avg $nop_avg $exit_flag
        done
    fi
fi
