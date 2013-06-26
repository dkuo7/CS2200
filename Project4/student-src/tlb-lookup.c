#include <stdlib.h>
#include <stdio.h>
#include "tlb.h"
#include "pagetable.h"
#include "global.h" /* for tlb_size */
#include "statistics.h"

/*******************************************************************************
 * Looks up an address in the TLB. If no entry is found, attempts to access the
 * current page table via cpu_pagetable_lookup().
 *
 * @param vpn The virtual page number to lookup.
 * @param write If the access is a write, this is 1. Otherwise, it is 0.
 * @return The physical frame number of the page we are accessing.
 */
pfn_t tlb_lookup(vpn_t vpn, int write) {
   pfn_t pfn;
   int i;
   tlbe_t *tlbe = NULL;

   /* 
    * FIX ME : Step 6
    */
   for(i = 0; i < tlb_size; i++) {
       if(tlb[i].valid && tlb[i].vpn == vpn) {
           count_tlbhits++;
           tlbe = tlb + i;
       }
   }
   /* 
    * Search the TLB for the given VPN. Make sure to increment count_tlbhits if
    * it was a hit!
    */
   
    
   /* If it does not exist (it was not a hit), call the page table reader */
   if(tlbe==NULL) {
       pfn = pagetable_lookup(vpn, write);
   }
   else {
       pfn = tlbe->pfn;
   }

   /* 
    * Replace an entry in the TLB if we missed. Pick invalid entries first,
    * then do a clock-sweep to find a victim.
    */
   if(tlbe==NULL) {
        for(i = 0; i < tlb_size; i++) {
            if(!tlb[i].valid) {
                tlbe = tlb + i;
                tlbe->vpn = vpn;
                tlbe->pfn = pfn;
            }
        } 
   }
   
   if(tlbe==NULL) {
       for(i = 0; i < tlb_size; i++) {
           if(!tlb[i].used) {
               tlbe = tlb + i;
               tlbe->vpn = vpn;
               tlbe->pfn = pfn;
               break;
           }
           tlb[i].used = 0;
       }
   }
   
   if(tlbe==NULL) {
       tlbe = tlb + 0;
       tlbe->vpn = vpn;
       tlbe->pfn = pfn;
   }

   /*
    * Perform TLB house keeping. This means marking the found TLB entry as
    * accessed and if we had a write, dirty. We also need to update the page
    * table in memory with the same data.
    *
    * We'll assume that this write is scheduled and the CPU doesn't actually
    * have to wait for it to finish (there wouldn't be much point to a TLB if
    * we didn't!).
    */
   if(write) {
       tlbe->dirty = 1;
       current_pagetable[vpn].dirty = 1;
   }
   tlbe->used = 1;
   current_pagetable[vpn].used = 1;

   return pfn;
}

