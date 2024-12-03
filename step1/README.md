```sh
$ cc -o 9cc 9cc.c
$ ./9cc 123 > tmp.s
$ cat tmp.s
$ cc -o tmp tmp.s
$ ./tmp
$ echo $?
```

