declblock {
   int ant,temp,i,j,temp;
   int arr[100];
}

codeblock {
  ant = 4;
  ant = ant + 3;
  temp = ant + 10;
  arr[14] = 12;
  
  println arr[14];

  for i=0,10
  {
     arr[i] = 9 - i;
  }

  i = 0;
  for i=0,10,1
  {
     j = 0;
     for j=0,9,1
     {
	if arr[j] > arr[j+1]
	{
	   temp = arr[j+1];
	   arr[j+1] = arr[j];
	   arr[j] = temp;
	}
     }
  }

  i=0;
  for i=0,10,1
  {
    print arr[i];
  }
}