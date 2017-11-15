#include "lcdconfig.h"
#include "msp.h"


void main(void)
{
    lcdconfig();
    getwordsback("Type this sentence",64,4);
    getwordsback("Type another",64,64);
    getwordsback("Realist Around",64,122);
    while(1);
}
