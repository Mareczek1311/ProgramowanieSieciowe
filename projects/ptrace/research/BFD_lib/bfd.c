#include <bfd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

int main(){
    
    long storage_needed, num_symbols, i;
    asymbol **symbol_table;

    bfd *abfd;

    char filename[100];

    bfd_init();

    abfd = bfd_openr(argv[1], NULL);
    assert(abfd != NULL);

    bfd_check_format(abfd, bfd_object);

    storage_needed = bfd_get_symtab_upper_bound(abfd);
    assert(storage_needed >= 0);

    printf("Storage needed: %d \n", storage_needed);

    symbol_table = (asymbol**) malloc (storage_needed);
    assert(symbol_table != 0);

    num_symbols = bfd_canonicalize_symtab(abfd, symbol_table);
    assert("num syms: %d \n", num_symbols);
    
    for(int i=0; i<num_symbols; i++){
        printf("%s: %x\n", bfd_asymbol_name(symbol_table[i]), 
            bfd_asymbol_value(symbol_table[i]));
    }


    return 0;
}

