# webserv
### This is when you finally understand why a URL starts with HTTP
<br/><br/><br/>

## Index
### [Summary](#summary)
### [Install](#install)
### [Usage](#usage)
### [UML diagram](#uml-diagram)
<br/><br/><br/>

## Summary
#### This project is about writing your ow HTTP server. You will be able to test it with an actual browser. HTTP is one of the most used protocols on the internet. Knowing its arcane will be useful, even if you won’t be working on a website.
<br/><br/><br/>

## Install
	$ make
<br/><br/><br/>

## Usage
	$ ./webserv conf
* Information about configuration file
<br/><br/><br/>

## UML diagram
### Class diagram
### ![Class diagram](./assets/Class%20diagram.png)
<br/>

### Sequence diagram
### ![Sequence diagram](./assets/Sequence%20diagram.png)

### 문제점
1. 유저 authentication 구현해야하는가? (cgi가 일반적으로 함)
2. nginx는 get요청 이외의 요청을 어떻게 처리하는가? (어케해야될지 모르겠음)