OPERATION = ['+', '-', "*", "/"]


def parse_infix(infix):
    op_stack = []
    postfix = []
    num = None
    for c in infix:
        if c.isnumeric():
            num = 10*num + int(c) if num is not None else int(c)
        elif c in OPERATION:
            if num is not None:
                postfix.append(num)
                num = None
                if len(op_stack) > 0 and op_stack[-1] in ['*', '/']:
                    postfix.append(op_stack.pop())
            op_stack.append(c)
        elif c == '(':
            op_stack.append(c)
        elif c == ')':
            if num is not None:
                postfix.append(num)
                num = None
            op = op_stack.pop()
            while(op != '('):
                postfix.append(op)
                op = op_stack.pop()
    if num is not None:
        postfix.append(num)
    while(len(op_stack) > 0):
        postfix.append(op_stack.pop())
    return postfix


def parse_postfix(postfix):
    lst = []
    num = None
    for c in postfix:
        if c.isnumeric():
            num = 10*num + int(c) if num is not None else int(c)
        elif c in OPERATION:
            if num is not None:
                lst.append(num)
                num = None
            lst.append(c)
        elif c.isspace():
            if num is not None:
                lst.append(num)
                num = None
    return lst


def calculate(postfix):
    stack = []
    for c in postfix:
        if str(c).isnumeric():
            stack.append(c)
        else:
            if len(stack) >= 2:
                y = stack.pop()
                x = stack.pop()
            if c == '+':
                stack.append(x + y)
            elif c == '-':
                stack.append(x - y)
            elif c == '*':
                stack.append(x * y)
            elif c == '/':
                stack.append(x / y)
    return stack[0]


def main():
    print("1. infix\n2. postfix")
    option = input("Please select a notaion: ")
    if int(option) == 1:
        infix = input("Input the infix notaion: ")
        postfix = parse_infix(infix)
        print("The postfix notaion: ", end='')
        for item in postfix:
            print(str(item), end=' ')
        print()
    else:
        postfix = input("Input the postfix notaion: ")
        postfix = parse_postfix(postfix)
    result = calculate(postfix)
    print("The value: " + str(result))


if __name__ == '__main__':
    main()
