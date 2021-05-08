#pragma once
#include <iostream>
#include <cstring>
#include <string>
#include "file.hpp"
#include "address.hpp"
#include "LRU.hpp"

#define PHYS_MEM_SIZE 65536
#define PAGE_TABLE_SIZE 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define SIMULATION_SIZE 1000 

class Manager
{
private:
    SBYTE physical_memory[PHYS_MEM_SIZE];
    int current_frame;

    int table[PAGE_TABLE_SIZE]; 
    int first;
    int page_faults, total_pages;
    LRU tlb;

    File* addys, *correct, *bstore;
public:
    Manager() : current_frame(0), first(0),
                        page_faults(0), total_pages(0) {
        addys   = new File("../data/addresses.txt");
        correct = new File("../data/correct.txt");
        bstore  = new File("../data/BACKING_STORE.bin");

        memset(table, -1, sizeof(table));
    }

   
    ~Manager()
    {
        delete addys;
        delete correct;
        delete bstore;
    }

    
    SBYTE getValue(int address)
    {
        LogicalAddress la(address);
        BYTE page_number = la.getPage(), offset = la.getOffset();
        int physical_addy;
        bool tlb_hit = tlb.contains(page_number);

        if(tlb_hit)
        { 
            printf("TLB Succeded\n");
            physical_addy = tlb[page_number] + offset;
        }
        else
        { 
            printf("Tlb failed\n");
            physical_addy = table[page_number];
           
            if(physical_addy == -1)
            {
                physical_addy = handlePageFault(page_number);
            } 
            ++total_pages;
           
            tlb.push(page_number, physical_addy);
            physical_addy += offset;
        }

        printf("Virtual address: %d Physical address: %d Value: %d ", 
                    address, physical_addy, physical_memory[physical_addy]);
        return physical_memory[physical_addy];
    }

    int handlePageFault(BYTE pnum)
    {
        printf("PAGE FAULT AT: %d\n", pnum);
        ++page_faults;
      
        int psize = PAGE_SIZE;
        int new_pa = current_frame * FRAME_SIZE;

        for(int x{}; x < psize; ++x)
        {
            physical_memory[new_pa + x] = (*bstore)[pnum*psize + x];
        }

        table[pnum] = new_pa;

        current_frame = (current_frame + 1) % FRAME_SIZE;
        return new_pa;
    }

   
    void simulate()
    {
        int overall{}, correct{};
        int value, cvalue;
        for(int x{}; x < SIMULATION_SIZE; ++x, ++overall)
        {
            value = this->getValue(addys->getAddress());
            cvalue = correct->parseValue();
            printf("Correct: %d\n", cvalue);
            if(value == cvalue) ++correct;
        }
        float accuracy = (float)correct/(float)overall;
        printf("----Accuracy: %.0f%%----", accuracy*100.0); 
        auto hits = tlb.getHits(), misses = tlb.getMisses();
        printf("TLB HITS: %d----TLB MISSES: %d----TLB-hit Rate: %.0f%%\n", hits, misses, (float(hits)/float(hits + misses))*100);
        printf("Page Fault Rate: %0.f%%", (float)page_faults/(float)correct * 100);
    }

    void test(int address)
    {
        LogicalAddress la(address);
        printf("page: %d, offset: %d\n", la.getPage(), la.getOffset());
    }
};
