- [Expressions](#expressions)
  - [Value Category◊](#value-category)
  - [Variables](#variables)
- [What `decaltype` does](#what-decaltype-does)


# Expressions

https://en.cppreference.com/w/cpp/language/expressions

Each C++ expression (an operator with its operands, a literal, a variable name, etc.) is characterized by two independent properties: a type and a value category. Each expression has some non-reference type, and each expression belongs to exactly one of the three primary value categories: *prvalue*, *xvalue*, and *lvalue*.

## Value Category◊

https://en.cppreference.com/w/cpp/language/value_category



> With the introduction of move semantics in C++11, value categories were redefined to characterize two independent properties of expressions[[5\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-5):
>
> - *has identity*: it's possible to determine whether the expression refers to the same entity as another expression, such as by comparing addresses of the objects or the functions they identify (obtained directly or indirectly);
> - *can be moved from*: [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor), [move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment), or another function overload that implements move semantics can bind to the expression.
>
> In C++11, expressions that:
>
> - have identity and cannot be moved from are called *lvalue* expressions;
> - have identity and can be moved from are called *xvalue* expressions;
> - do not have identity and can be moved from are called *prvalue* ("pure rvalue") expressions;
> - do not have identity and cannot be moved from are not used[[6\]](https://en.cppreference.com/w/cpp/language/value_category#cite_note-6).
>
> The expressions that have identity are called "glvalue expressions" (glvalue stands for "generalized lvalue"). Both lvalues and xvalues are glvalue expressions.
>
> The expressions that can be moved from are called "rvalue expressions". Both prvalues and xvalues are rvalue expressions.



```
   ______ ______

  /      X      \

 /      / \      \

|   l  | x |  pr  |

 \      \ /      /

  \______X______/

​      gl    r
```



Above diagram describes the general relationship between *lvalue*(l), *xvalue*(x), *prvalue*(pr), *glvalue*(gl), *rvalue*(r)

# Universal Reference

https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers

https://accu.org/journals/overload/20/111/overload111.pdf#page=9

## Key Points & Golden Rules

- Remember that “&&” indicates a universal reference only where type deduction takes place.  Where there’s no type deduction, there’s no universal reference.  In such cases, “&&” in type declarations always means rvalue reference.
- lvalueness or rvalueness of an expression is independent of its type, it’s possible to have lvalues whose type is rvalue reference, and it’s also possible to have rvalues of the type rvalue reference
- Apply std::move to rvalue references and std::forward to universal references
- Only use std::forward with universal references
- Universal reference type deduction is the only situation a template parameter is deduced as reference(when passed type is of lvalue). This rule is not applicable to `const`.


## Variables

We use expression to declare variables, only variables(has name) is related to reference. Expressions has nothing to do
with reference yet. Reference is alias for existing object, and do not have object(memeory occupation) itself, which means
reference must have a name(reference variable name), otherwise it can not exit, since it's alias. A name and a binding
object are all reference have and must have.

So for expression, let's say it has value type and value category; For variables, which is delcared using expression and
must have names, let's say it might be of value type or reference type, which consists of lvalue reference and rvalue
reference.

Named variable is also one kind of expression, it has lvalue value type and non-reference type; What is special about
named variables is that it is always lvalue and except non-reference types, it might be reference to existing object. In
this case, the non-reference type of the existing object is the type of the variable and the variable itself is also a
reference. It is extremely confusing, what must be remembered is that value category has no relationship with reference
whatsoever. A variable can be lvalue and reference to lvalue at the same time, which is called lvalue reference; A
variable can also be lvalue and reference to rvalue at the same time, which makes the variable rvalue reference. When we
say a variable is lvalue or rvalue reference, we mean the expression that initialize this variable is lvalue or rvalue,
for the variable itself, it's always lvalue. Considering all this, a named variable expression has three properties:
1. non-reference type: the type of the existing object, whether the variable directly points to or the variable refers to
2. reference type: if the variable is a reference, this is either lvalue reference or rvalue reference
3. value category: always lvalue category



# What `decaltype` does

Inspects the declared type of an entity or the type and value category of an expression.
This implies two funtionality of decltype:

1. When used as decltype ( entity ), where entity is unparenthesized id-expression or class memeber expression, it yields
the type of entity
2. When used as decltype ( expression ), where expression is any other expression, it inspects the expression's
value type(eg,T) and value category and yields following types accordingly:

- if value category of expression is xvalue, it yields T&&
- if value category of expression is lvalue, it yields T&
- if value category of expression is prvalue, it yields T

Note: if variable id-expression or class memeber access expression is parenthesized, it is treated as ordinary lvalue
expression(which is reasonable, because named variables are always lvalue expressions)

How to use decltype to defer the value category of expression

First approach, we can check whether the yield type of decltype(expression) is lvalue or rvalue reference, if rvalue
reference, the expression is xvalue; if lvalue reference, the expression is lvalue; otherwise, the expression is prvalue
