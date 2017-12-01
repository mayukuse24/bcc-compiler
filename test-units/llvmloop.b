declblock {
   int ant,temp;
}

codeblock {
  ant = 4;
  ant = ant + 3;
  temp = ant + 10;

  print "hey there %d",temp;

  while ant<8
  {
    ant = ant + 1;
  }

  print "testing loop works %d",ant;
}