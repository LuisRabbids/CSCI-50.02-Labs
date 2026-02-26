#include <iostream>
#include "IntArray.h"
using namespace std;

int main(int argc, char *argv[])
{
    string input = argv[1];
    int x = atoi(argv[1]);

    cout << "\t.file\t\"multiplyBy" << x <<".cpp\"" << endl;
    cout << "\t.text" << endl;
    cout << "\t.globl\t_Z" << 10 + input.length() << "multiplyBy" << x << "P8IntArrayi" << endl;
    cout << "\t.type\t_Z" << 10 + input.length() << "multiplyBy" << x << "P8IntArrayi, @function" << endl;
    cout << "_Z" << 10 + input.length() << "multiplyBy" << x << "P8IntArrayi" << endl;

    cout << ".LFB0:" << endl;
    cout << "\t.cfi_startproc" << endl;
    cout << "\tendbr64" << endl;

    cout << "\tpushq\t%rbp" << endl;
    cout << "\t.cfi_def_cfa_offset\t16" << endl;
    cout << "\t.cfi_offset\t6, -16" << endl;
    cout << "\tmovq\t%rsp, %rbp" << endl;
    cout << "\t.cfi_def_cfa_register 6" << endl;
    cout << "\tmovq\t%rdi, -24(%rbp)" << endl;
    cout << "\tmovl\t\%esi, -28(%rbp)" << endl;
    cout << "\tmovq\t-24(%rbp), %rax" << endl;
    cout << "\tmovl\t(%rax), \%eax" << endl;
    cout << "\tmovl\t\%eax, -12(%rbp)" << endl;
    cout << "\tmovq\t-24(%rbp), %rax" << endl;
    cout << "\tmovq\t8(%rax), %rax" << endl;
    cout << "\tmovq\t%rax, -8(%rbp)" << endl;
    cout << "\tmovl\t$0, -16(%rbp)" << endl;
    cout << "\tjmp .L2" << endl;

    cout << ".L3:" << endl;
    cout << "\tmovl\t-16(%rbp), \%eax" << endl;
    cout << "\tcltq" << endl;
    cout << "\tleaq\t0(,%rax,4), %rdx" << endl;
    cout << "\tmovq\t-8(%rbp), %rax" << endl;
    cout << "\taddq\t%rdx, %rax" << endl;
    cout << "\tmovl\t(%rax), \%eax" << endl;
    cout << "\tmovl\t-16(%rbp), \%edx" << endl;
    cout << "\tmovslq\t\%edx, %rdx" << endl;
    cout << "\tleaq\t0(,%rdx,4), %rcx" << endl;
    cout << "\tmovq\t-8(%rbp), %rdx" << endl;
    cout << "\taddq\t%rcx, %rdx" << endl;
    cout << "\timull\t$" << x << ", \%eax, \%eax" << endl;
    cout << "\tmovl\t\%eax, (%rdx)" << endl;
    cout << "\taddl\t$1, -16(%rbp)" << endl;

    cout << ".L2:" << endl;
    cout << "\tmovl\t-16(%rbp), \%eax" << endl;
    cout << "\tcmpl\t-12(%rbp), \%eax" << endl;
    cout << "\tjl\t.L3" << endl;
    cout << "\tnop" << endl;
    cout << "\tnop" << endl;
    cout << "\tpopq\t%rbp" << endl;
    cout << "\t.cfi_def_cfa 7, 8" << endl;
    cout << "\tret" << endl;
    cout << "\t.cfi_endproc" << endl;

    cout << ".LFE0:" << endl;
    cout << "\t.size\t_Z" << 10 + input.length() << "multiplyBy" << x << "P8IntArrayi, .-_Z" << 10 + input.length() << "multiplyBy" << x << "P8IntArrayi" << endl;
    cout << "\t.ident\t\"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0\"" << endl;
    cout << "\t.section\t.note.GNU-stack,\"\",@progbits" << endl;
    cout << "\t.section\t.note.gnu.property,\"a\"" << endl;
    cout << "\t.align 8" << endl;
    cout << "\t.long\t1f - 0f" << endl;
    cout << "\t.long\t4f - 1f" << endl;
    cout << "\t.long\t5" << endl;

    cout << "0:" << endl;
    cout << "\t.string\t\"GNU\"" << endl;

    cout << "1:" << endl;
    cout << "\t.align 8" << endl;
    cout << "\t.long\t0xc0000002" << endl;
    cout << "\t.long\t3f - 2f" << endl;

    cout << "2:" << endl;
    cout << "\t.long\t0x3" << endl;
    
    cout << "3:" << endl;
    cout << "\t.align 8" << endl;

    cout << "4:" << endl;
    
    return 0;
}

// https://learn.microsoft.com/en-us/cpp/cpp/main-function-command-line-args?view=msvc-170
// https://www.geeksforgeeks.org/cpp/cpp-program-for-char-to-int-conversion/
// https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling