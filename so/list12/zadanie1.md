# Zadanie 1

```
1 __thread long myid;
2 static char **strtab;
3
4 void *thread(void *vargp) {
5   myid = *(long *)vargp;
6   static int cnt = 0;
7   printf("[%ld]: %s (cnt=%d)\n", myid, strtab[myid], ++cnt);
8   return NULL;
9 }
10
11 int main(int argc, char *argv[]) {
12   ...
13   strtab = argv;
14   while (argc > 0) {
15     myid = --argc;
16     pthread_create(&tid, NULL, thread, (void *)&myid);
17   }
18   ...
19 }
```

`myid` - nie współdzielone
`strtab` - współdzielone
`vargp` - nie współdzielone
`cnt` - współdzielone
`argc` - nie współdzielone
`argv[0]` - współdzielone

data race występuje w `cnt` (wiele wątków może naraz ++cnt).

