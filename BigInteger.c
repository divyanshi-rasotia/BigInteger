#include"BIGINTEGER.H"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct node* createNode(int val) 
{
    struct node *newNode = (struct node*)malloc(sizeof(struct node));
    if (!newNode) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = val;
    newNode->next = NULL;
    return newNode;
}
void addAtBeginning(struct BigInteger *bigint, int data) 
{
    struct node *newNode = createNode(data);
    newNode->next = bigint->L;
    bigint->L = newNode;
    bigint->length++;
}

void truncate(struct BigInteger *num) 
{
    if (num == NULL || num->L == NULL) 
    {
        return;
    }

    struct node *current = num->L;
    struct node *lastNonZero = NULL;

    while (current != NULL) 
    {
        if (current->data != 0) 
        {
            lastNonZero = current;
        }
        current = current->next;
    }

    if (lastNonZero == NULL) 
    { 
        current = num->L;
        while (current->next != NULL) 
        {
            struct node *temp = current;
            current = current->next;
            free(temp);
        }
        num->L = createNode(0);
        num->length = 1;
    } 
    else if (lastNonZero->next != NULL) 
    {
        current = lastNonZero->next;
        lastNonZero->next = NULL;

        while (current != NULL) 
        {
            struct node *temp = current;
            current = current->next;
            free(temp);
            num->length--;
        }
    }
}



struct BigInteger initialize(const char *s) {
    struct BigInteger t;
    t.L = NULL;
    t.sign = 0;

    int x = strlen(s);
    t.length = x - (s[0] == '-');
    if (s[0] == '-') {
        t.sign = 1;
    }

    for (int i = 0; i<=x-1; i++) {
        int digit = s[i] - '0';
        struct node* newNode = createNode(digit);
        newNode->next = t.L;
        t.L = newNode;
    }

    return t;
}

int compareBigINT(struct BigInteger b1, struct BigInteger b2) 
{
    int res=0;
    if (b1.length > b2.length) return 1;
    if (b1.length < b2.length) return -1;
    struct node* current1 = b1.L;
    struct node* current2 = b2.L;
    while (current1 && current2) 
    {
        if (current1->data > current2->data) 
        res=1;;
        if (current1->data < current2->data) 
        res= -1;
        current1 = current1->next;
        current2 = current2->next;
    }
    return res;
}


struct BigInteger add(struct BigInteger b1, struct BigInteger b2) {
    struct BigInteger result;
    result.L = NULL;
    result.length = 0;
    result.sign = 1;

    if (b1.sign != b2.sign) 
    {
        b2.sign *= -1;
        return sub(b1, b2);
    } 
    else 
    {
        int carry = 0;
        struct node *current1 = b1.L;
        struct node *current2 = b2.L;
        struct node *temp = NULL;

        while (current1 || current2 || carry) {
            int val1 = (current1) ? current1->data : 0;
            int val2 = (current2) ? current2->data : 0;
            int sum = val1 + val2 + carry;
            carry = sum / 10;
           
            sum = sum % 10;
            if (result.L == NULL) 
            {
                result.L = createNode(sum);
                temp = result.L;
            } 
            else 
            {
                temp->next = createNode(sum);
                temp = temp->next;
            }

            result.length++;

            if (current1) current1 = current1->next;
            if (current2) current2 = current2->next;
        }

        result.sign = b1.sign;
    }
    
    return result;
}

struct node* reverseList(struct node* head) {
    struct node* prev = NULL;
    struct node* current = head;
    struct node* next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}

void display(struct BigInteger num) {
    if (num.L == NULL) return;

    
    num.L = reverseList(num.L);
    
    if (num.sign) {
        printf("-");
    }
    
    struct node* current = num.L;
    while (current != NULL) {
        printf("%d", current->data);
        current = current->next;
    }
    printf("\n");
}

struct BigInteger negate(struct BigInteger num) 
{
    num.sign = 1 - num.sign;
    return num;
}



struct BigInteger sub(struct BigInteger b1, struct BigInteger b2) {
    struct BigInteger result;
    int n;
    result.L = NULL;
    result.length = 0;
    result.sign = 1;
    
    if (b1.sign != b2.sign) {
        b2.sign *= -1;
        return add(b1, b2);
    } else {
        int swap = 1;
        n=compareBigINT(b1, b2);
        if (n == -1) 
        {
            struct BigInteger temp = b1;
            b1 = b2;
            b2 = temp;
            swap = -1;
        }

        int borrow = 0;
        struct node* current1 = b1.L;
        struct node* current2 = b2.L;
        struct node* prev = NULL;

        while (current1 || current2) {
            int val1 = (current1) ? current1->data : 0;
            int val2 = (current2) ? current2->data : 0;
            int diff = val1 - val2 - borrow;

            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }

            if (result.L== NULL) {
                result.L = createNode(diff);
                prev = result.L;
            } else {
                prev->next = createNode(diff);
                prev = prev->next;
            }

            result.length++;

            if (current1) current1 = current1->next;
            if (current2) current2 = current2->next;
        }

        result.sign = swap * b1.sign;
    }
    
    truncate(&result);
    return result;
}


void freeBigInteger(struct BigInteger* num) {
    struct node* current = num->L;
    while (current != NULL) {
        struct node* next = current->next;
        free(current);
        current = next;
    }
    num->L = NULL;
    num->length = 0;
    num->sign = 0;
}

struct BigInteger mul(struct BigInteger num1, struct BigInteger num2) {
    struct BigInteger result = initialize("0");
    
    struct node* current1 = num1.L;
    struct node* current2 = num2.L;
    int shift = 0;
    
    while (current1 != NULL) {
        struct BigInteger tempResult = initialize("0");
        int carry = 0;
        
        for (int i = 0; i < shift; i++) {
        addAtBeginning(&tempResult, 0);
        }
        current2 = num2.L;
        while (current2 != NULL || carry > 0) {
            int product = ((current2 != NULL) ? current2->data : 0) * current1->data + carry;
            carry = product / 10;
            product = product % 10;
           addAtBeginning(&tempResult, product);
            
            if (current2 != NULL) {
                current2 = current2->next;
            }
        }
        
        result = add(result, tempResult);
        current1 = current1->next;
        shift++;
    }
    result.L=reverseList(result.L);
    return result;
}

struct BigInteger mod(struct BigInteger b1, struct BigInteger b2) {
    if (compareBigINT(b2, initialize("0")) == 0) {
        printf("Error: Division by zero\n");
        exit(EXIT_FAILURE);
    }

    struct BigInteger zero = initialize("0");
    if (compareBigINT(b1, zero) == 0) {
        return zero;
    }

    int b1Sign = b1.sign;
    int b2Sign = b2.sign;
    b1.sign = b2.sign = 0;
    while (compareBigINT(b1, b2) != -1) {
        b1 = sub(b1, b2);
    }
    b1.sign = b1Sign;
    b2.sign = b2Sign;
    return b1;
}


