declblock {
   int ant,temp;
   int i,j,k;
}

codeblock {
  ant = 4;
  ant = ant + 3;
  temp = ant + 10;

  
  print "hey there %d \n",temp;

  i=0;
  while i<100
  {
    j = 0;
    while j<100
    {
	k = 0;
	while k<100
	{
	   ant = ant + 1;
	   k = k+1;
	}
	j = j+1;
    }
    i=i+1;
  }

  print "testing loop works %d",ant;
}