declblock {
   int ant,temp,i,j,k,temp;
   int arr[100];
}

codeblock {
  ant = 0;

  for i=0,100,1
  {
      for j=0,100,1
      {
	 for k=0,100,1
	 {
	    if k>50
	    {
		ant =ant + 1;
	    }
	    else
	    {
		ant = ant + 2;
	    }	
	 }	 
      }
  }
  print "aksjdfkd",ant;
}
