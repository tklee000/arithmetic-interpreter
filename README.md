# arithmetic-interpreter

간단한 사칙연산 C++ 인터프리터

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
```
  expression = term, { "+" | "-", term }
  expression은 term 의 결과가 바로 될수도 있다. 혹은 다음 토큰이 +- 인경우 다음 term 과 연산한다.
  예1) 1*3 인경우는 term 이 바로 expression 이되며, expression 내부의 로직은 스킵
  예2) 1+2 인경우에도 term 을 Call 하지만 term 내부의 로직은 스킵되고 expression = factor로 된다...
  예3) 1+2*3 인 경우 2*3 이 먼저 연산되는 과정 설명
       1 + 까지 처리후
	   1 + X   --> X term을 Call (while 루프안)
	   term은 2*3 을 처리후 return
	   expression 함수가 완료되지 않은채 중간에서 Call을 했기 때문에 
	   term 결과를 기다리면서 expression 은 보류된다. 
	   expression 을 처리하다 중간 term 함수호출으로 인해서
	   term 함수 안의 연산이 먼저 처리되어서 현재 연산이 보류된다.
	   중간 함수 호출로 연산 보류가 즉 우선순위를 바꾸는 결과가 된다.
	   참고로 EBNF 문법에서 연산우선 순위가 있는 경우 위와 같은 문법으로 처리가능하다.
	   term = factor, { "*" | "/", factor }
	   expression = term, { "+" | "-", term }
	   term, expression 은 각각 처리 함수를 만들어 처리해야 하며, 함수 Call 형식으로 작성해야 한다.
   예4) 2*(3+1) 인 경우 (3+1) 이 먼저 연산되는 과정 설명
	   2 * 까지 처리후
	   2 * X   --> X factor을 Call (term while 루프안)
	   factor는 ( 를 만나서 새로운 expression 을 처리하도록 함수 call expression
	   term 함수가 완료되지 않은채 call factor 
	   factor 또한 함수가 완료되지 않은채 call expression
	   두번 연속 함수 Call 되는 상황임
	   factor에서 call한 expression 이 완료되고 factor로 전부 완료되기 전까지는 term 도 보류~~
```     
