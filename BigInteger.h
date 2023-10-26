#ifndef BIGINTEGER_H
#define BIGINTEGER_H
struct node 
{
    int data;
    struct node *next;
};
struct BigInteger
{
    struct node *L;
    int length;
    int sign;
};
struct BigInteger initialise(const char*);
struct BigInteger add(struct BigInteger,struct BigInteger);
struct node* createNode(int);
struct BigInteger sub(struct BigInteger, struct BigInteger);
void display(struct BigInteger);
int compareBigINT(struct BigInteger, struct BigInteger);
void truncate(struct BigInteger *);
void freeBigInteger(struct BigInteger*);
struct BigInteger negate(struct BigInteger);
struct BigInteger mul(struct BigInteger , struct BigInteger);
void addAtBeginning(struct BigInteger*, int);
struct node* reverseList(struct node*);
struct BigInteger mod(struct BigInteger, struct BigInteger);
#endif