#ifndef WALK_PAGE_TABLE_H
#define WALK_PAGE_TABLE_H
#include "walk_page_table.h"
#endif

int walk_page_table(unsigned long addr) {
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *ptep, pte;

    struct page *page = NULL;
    struct mm_struct *mm = current->mm;

    pr_alert("Address received for page walk: 0x%lx\n", addr);

    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd)) {
        goto finish;
    }
    pr_notice("PGD is valid");

    pud = pud_offset(pgd, addr);
    if (pud_none(*pud) || pud_bad(*pud)) {
        goto finish;
    }
    pr_notice("PUD is valid");

    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd)) {
        goto finish;
    }
    pr_notice("PMD is valid");

    ptep = pte_offset_map(pmd, addr);
    if (!ptep) {
        goto finish;
    }
    pte = *ptep;

    page = pte_page(pte);
    if (page) {
        pr_info("Page frame location: %p", page);
    }

    pte_unmap(ptep);

finish:
    return 0;
}

