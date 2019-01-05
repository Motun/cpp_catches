/* 
 * https://quuxplusone.github.io/blog/2019/01/03/const-is-a-contract/ 
 */

#include <iostream>

/* Define a global int variable for experimental purposes. */
int global = 2;

/* Forward declarations */
void taking_const_ref(const int& arg);
void post_increment(int& arg);
void do_nothing(const int& arg);

int main(int argc, char **argv)
{
    /* cint -defined as const- is truly const. */
    const int cint = 47;
    std::cout << "cint: " << cint << std::endl;
    //cint++; // Compiler error

    /* A reference for const_cast: https://en.cppreference.com/w/cpp/language/const_cast */
    /* In fact, it's not allowed to const_cast a variable if the variable is actually const. 
    Refer to this stackoverflow answer: https://stackoverflow.com/a/19554871/3160184 */
    const_cast<int&>(cint) = 48; // Undefined behaviour
    std::cout << "[Undefined behaviour] cint: " << cint << std::endl;

    //int *p_cint = &cint; // Compiler error
    //const int *p_cint = &cint; // Okay
    //int const *p_cint = &cint; // Same as above

    /* i isn't const, but we can (kind of) make it so through a reference or a pointer */
    int i = 17; // Mutable
    std::cout << "i initial: " << i << std::endl;
    const int& ref_i = i; // Reference to a const int (target isn't actually const)
    const int* ptr_i = &i; // Pointer to a const int (target isn't actually const)

    //ref_i = 19; // Compiler error
    //*ptr_i = 19; // Compiler error;

    /* Since the real target (i) isn't const, we can strip away these consts. */
    const_cast<int&>(ref_i) = 19;
    std::cout << "i after const_cast<int&>: " << i << std::endl;
    *const_cast<int*>(ptr_i) = 11;
    std::cout << "i after const_cast<int*>: " << i << std::endl;

    /* Function arguments */
    std::cout << "global before func call: " << global << std::endl;
    taking_const_ref(global);
    std::cout << "global after func call: " << global << std::endl;

    /*
     * About the placement of const keyword
     * ------------------------------------
     * 
     * An excerpt from https://quuxplusone.github.io/blog/2019/01/03/const-is-a-contract/:
     * 
     * -- Pointers behave just like references in this respect --
     * 
     * When we take a function parameter of type const int *, we’re saying “Please give me
     * the address of an int, and I promise not to modify that int.”
     * 
     * And the const can apply at each different level, independently:
     * When we take a function parameter of type const int **, we’re saying “Please give me
     * a pointer to an int*, and I promise not to modify the int it points to (but I might
     * modify the int* itself).”
     * 
     * When we take a function parameter of type int *const *, we’re saying “Please give me
     * a pointer to a pointer to an int, and I promise not to modify that int* (but I might
     * modify the int it points to).”
     * 
     * When we take a function parameter of type const int *const *, we’re saying “Please
     * give me a pointer to an int*, and I promise not to modify either the int* or the int
     * it points to.”
     * 
     * (Very minor handwaving here: “pointer to int*” and “pointer to const int*” are actually
     * different types, and behave according to covariance and contravariance, which is fun
     * times. Intrepid readers can find more details in http://eel.is/c++draft/conv.qual#3,
     * until I find a better explanation to link to.)
     */

    /* Prefer "int const" declaration to "const int" because it'll be easier to figure out
    what is const and what isn't. Look to the left of the const keyword, that is the const. */
    int const *p_intc = &cint; // Pointer to a constant integer, this is okay
    std::cout << "Value pointed by p_intc: " << *p_intc << std::endl;
    //*p_intc = 7; // Compiler error

    int const new_cint = 13;
    p_intc = &new_cint; // Okay, the pointer itself isn't const
    std::cout << "Value pointed by p_intc: " << *p_intc << std::endl;

    int * const pc_int = &global;
    std::cout << "Value pointed by pc_int: " << *pc_int << std::endl;
    *pc_int = 6; // Okay
    std::cout << "Value pointed by pc_int: " << *pc_int << std::endl;
    //pc_int = &i; // Compiler error, the value pointed isn't const but the pointer itself is

    int const * const pc_intc = &cint; // Constant pointer to a constant integer, also okay
    //*pc_intc = 7; // Compiler error
    //pc_intc = &new_cint; // Compiler error, the pointer itself is const, too

    return 0;
}

/* This function also promises not to modify its argument. */
void do_nothing(const int& arg){}

/* A function taking a reference to a const int. */
/* That is, we promise to not modify the variable arg. */
void taking_const_ref(const int& arg)
{
    printf("%d ", arg);
    //arg = 3 // Compiler error because of the promise

    /* We can strip away the constness because the underlying variable isn't actually const. */
    const_cast<int&>(arg) = 3;
    printf("%d ", arg);

    global = 5;
    printf("%d\n", arg);

    //post_increment(arg); // Compiler error because this function modifies arg
    //do_nothing(arg); // Okay because this function promises not to modify arg
}

void post_increment(int& arg)
{
    arg++;
}


