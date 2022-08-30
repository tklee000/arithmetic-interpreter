# arithmetic-interpreter

간단한 사칙연산 인터프리터 C++

1+2+4+((1+2)*3)/3=10

```
factor = ( [ "+" | "-" ], integer ) | ( "(", expression, ")" )
term = factor, { "*" | "/", factor }
expression = term, { "+" | "-", term }
```

* expression을 처음으로 시작하고 '(' 가 나오면 expression 생성

* factor는 상수를 가지고 올수 있으며 괄호를 만나면 새로운 expression 으로 Call 
* term = factor 끼리 (*)(/) 연산
* expression은 term 끼리 (+)(-) 연산

* expression 에서 term을 만나면 term을 먼저 함수로 돌리기 때문에 자연스레 (+)(-) 보다는 term 쪽 (*)(/) 연산이 먼저 처리되서 결과를 받아서 express에서 처리된다.
