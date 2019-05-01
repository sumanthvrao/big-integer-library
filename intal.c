#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intal.h"

typedef struct dtype
{
	char* string; //to store the string 
	int length;// stores the length associated with every string.
}dtype;

//static void* karatsuba(void* a,void* b); //did not work
//static void* russian_peasant_multiply(void* intal1,void* intal2) //was too slow.
static void* intal_pow_recursive(void* intal1 ,void* intal2,void* two);

static int is_odd(void* number);

void* intal_create(const char* str)
{
	if(str==NULL)
	{
		dtype* copy = (dtype*)malloc(sizeof(dtype));
		copy->string = (char*)malloc(sizeof(char)*1);
		copy->length = 1;
		copy->string[0]='0';
		return (void*)copy; //NULL Pointer
	}
	int len = strlen(str);
	if(len==0) //checking if string entered is empty.
	{
		dtype* copy = (dtype*)malloc(sizeof(dtype));
		copy->string = (char*)malloc(sizeof(char)*1);
		copy->length = 1;
		copy->string[0]='0';
		return (void*)copy;
	}   
		
	if(str[0]<48 || str[0]>57)
		{
		dtype* copy = (dtype*)malloc(sizeof(dtype));
		copy->string = (char*)malloc(sizeof(char)*1);
		copy->length = 1;
		copy->string[0]='0';
		return (void*)copy;	
		}
	int i,count=0;
	for(i=0;i<len;i++)
	{

		if(str[i]!='0')
			break;
		count++;
	}
	len = len - count; //removing padded zeroes at the intal_create stage itself.
	if(len==0) //creates a single 0 . 
	{
		dtype* copy = (dtype*)malloc(sizeof(dtype));
		copy->string = (char*)malloc(sizeof(char)*1);
		copy->length = 1;
		copy->string[0]='0';
		return (void*)copy;
	}

	dtype* copy = (dtype*)malloc(sizeof(dtype));
	copy->string = (char*)malloc(sizeof(char)*len);
	copy->length = len;

	for(i=0;i<len;i++)
	{
		if(str[i]<48 || str[i]>57)
		{
			copy->string = (char*)realloc(copy->string , sizeof(char)*(i+1)); //first encounters a non number character
			copy->length = i;
			copy->string[copy->length]='\0';
			return (void*)copy;
		}
		copy->string[i]=str[count++];
	}
	return (void*)copy;
}


void* intal_increment(void* intal)
{
	if(intal==NULL)
		return NULL;
	dtype* k = (dtype*)intal;
	int nlen = k->length;
	int flag = 1;
	for(int i=0;i<nlen;i++)
	{
		if(k->string[i]!='9') //counting the 9's from the right.
		{
			flag =0;
			break;
		}
	}
	if(flag == 1)
	{
		dtype* new = (dtype*)malloc(sizeof(dtype));
		new->string = (char*)malloc(sizeof(char)*(nlen+1)); // creates a new copy since it overflows.
		new->length = nlen+1;
		new->string[0]='1';
		for(int i =1;i<=nlen;i++)
		{
			new->string[i]='0';
		}
		intal_destroy(intal); //destroys the old object
		return (void*)new;
	}
	if(flag == 0)
	{
		//printf("%d\n",nlen);
		if(k->string[nlen-1]!='9')
		{
			k->string[nlen-1]=(char)((int)k->string[nlen-1]+1);
			return (void*)k;
		}
		else
		{
			int l=0;
			for(l=nlen-1;l>=0;l--)
			{
				if(k->string[l]!='9')
					break;
			}
			k->string[l]=(char)((int)k->string[l]+1);
			for(int f=l+1;f<nlen;f++)
			{
				k->string[f]='0';
			}
			return (void*)k;
		}
	}
}

void* intal_decrement(void* intal)
{
	if(intal==NULL)
		return NULL;
	dtype* k = (dtype*)intal;
	int nlen = k->length;
	int flag = 1;

	if(k->string[0]=='0') //if the input is zero NOP done.
	{
		return intal;
	}
	else if(k->string[0]=='1' && k->length==1)
	{
		intal_destroy(intal); //destroys the old object
		return intal_create("0"); //if the input is a single one.
	}

	if(k->string[0]!='1')
		flag =0;
	for(int i=1;i<nlen;i++)
	{
		if(k->string[i]!='0')
		{
			flag =0;
			break;
		}
	}
	if(flag == 1)
	{
		dtype* new = (dtype*)malloc(sizeof(dtype));
		new->string = (char*)malloc(sizeof(char)*(nlen-1)); //creates a new copy by decreasing its length
		new->length = nlen-1;
		for(int i =0;i<nlen-1;i++)
		{
			new->string[i]='9';
		}
		intal_destroy(intal); //destroys the old object
		return (void*)new;
	}
	if(flag == 0)
	{
		
		//printf("%d\n",nlen);
		if(k->string[nlen-1]!='0')
		{
			k->string[nlen-1]=(char)((int)k->string[nlen-1]-1);
			return (void*)k;
		}
		else
		{
			int l=0;
			for(l=nlen-1;l>=0;l--)
			{
				if(k->string[l]!='0')
					break;
			}
			k->string[l]=(char)((int)k->string[l]-1);
			for(int f=l+1;f<nlen;f++)
			{
				k->string[f]='9';
			}
			return (void*)k;
		}
	}
 }


void* intal_add(void* intal1, void* intal2)
{
	if(intal1== NULL || intal2 == NULL)
		return NULL;
	dtype* num1 =(dtype*)intal1;
	dtype* num2 = (dtype*)intal2;

	if(num1->string[0]=='0')
	{
		return intal_create(num2->string);              // one of the number is a 0.
	}

	else if(num2->string[0]=='0')
	{
		return intal_create(num1->string);              // one of the number is a 0.
	}

	int max = num1->length > num2->length ? num1->length:num2->length;

	dtype* sum = (dtype*)malloc(sizeof(dtype));
	sum->string = (char*)malloc(sizeof(char)*(max));
	sum->length = max; //length of sum can never be lesser than the longest number.

	if(num1->length > num2->length)
	{
		dtype* temp=num2;
		num2=num1;
		num1=temp;
	}
	

	if(num1->length < num2 -> length)
	{
		int i;
		int j=num2->length-1;
		int carry=0;
		for(i=num1->length-1;i>=0;i--)
		{
			int sm = (num1->string[i]-'0')+(num2->string[j]-'0')+carry;
			carry=0;
			if(sm>9)
			{
				carry=1;
				sm = sm -10;
			}

			//printf("%d %d\n",carry,sm);
			sum->string[j]=sm+'0';
			j--;
		}
		while(j>=0)
		{
			int sm = carry + (num2->string[j]-'0');
			carry=0;
			if(sm>9)
			{
				carry=1;
				sm = sm-10;
			}
			//printf("---%d %d\n",carry,sm);
			sum->string[j]=sm+'0';
			if(j==0 && carry==1)
			{
				//printf("Entered\n");
				sum->string = (char*)realloc(sum->string,max+1); //if it overflows the length.
				sum->length = sum->length+1;
				for(int k = max;k>0;k--)
				{
					sum->string[k]=sum->string[k-1];
				}
				sum->string[0]='1';
				break;
			}
			j--;
		}
	}

	else
		{
			int i;
			int j=num2->length-1;
			int carry=0;
			for(i=num1->length-1;i>=0;i--)
			{
				int sm = (num1->string[i]-'0')+(num2->string[j]-'0')+carry;
				carry=0;
				if(sm>9)
			{
				carry=1;
				sm = sm -10;
			}

			//printf("%d %d\n",carry,sm);
				sum->string[j]=sm+'0';
				j--;
			}
		if(carry==0)
			return (void*)sum;
		else
			{
				sum->string = (char*)realloc(sum->string,sum->length+2);
				sum->length =sum->length+1;
				for(int k = max;k>0;k--)
				{
					sum->string[k]=sum->string[k-1];
				}
				sum->string[0]='1';
				sum->string[max+1]='\0';
			}
		
		}
		//printf("%s\n",sum->string);
	//printf("The length %d\n",sum->length);
	return (void*)sum;
}

void* intal_diff(void* intal1, void* intal2)// a-b
{
	if(intal1==NULL || intal2==NULL)
		return NULL;
	dtype* num1 = (dtype*)intal1;
	dtype* num2 = (dtype*)intal2;

	if(num1->string[0]=='0')
	{
		return intal_create(num2->string);              // one of the number is a 0.
	}

	else if(num2->string[0]=='0')
	{
		return intal_create(num1->string);              // one of the number is a 0.
	}

	dtype* diff = (dtype*)malloc(sizeof(dtype));
	diff->string = (char*)malloc(sizeof(char)*(num1->length));
	diff->length = num1->length;

	int x=intal_compare(intal1,intal2);
	if(x==-1)
	{
		return intal_diff(intal2,intal1); //ensuring the difference is not Negative.
	}
	int i,j;
	int borrow=0;
	int difference = num1->length - num2->length;
	int count=num1->length-1;
	for(i=num2->length-1;i>=0;i--)
	{
		int df = ((num1->string[i+difference]-'0')-(num2->string[i]-'0')-borrow);
		borrow =0;
		if(df <0)
		{
			borrow =1;
			df = df + 10;
		}
		//printf("%d %d\n",df,borrow);
		diff->string[count]=df+'0';
		//printf("count %d\n",count);
		count--;
		
	}

	for(j=(num1->length)-(num2->length)-1;j>=0;j--)
	{
		//printf("Entered\n");
		if(j==0 && borrow ==1)
		{
			diff->string[count]=(char)((int)num1->string[j]-1);
			//printf("count %d\n",count);
			count--;
			break;
		}
		if(num1->string[j]=='0' && borrow==1)
		{
			diff->string[count]='9';
			//printf("count %d\n",count);
			count--;
			continue;
		}

		int df = ((num1->string[j]-'0')-borrow);
		//printf("%d %d\n",df,borrow);
		if(j>=0 || borrow > 0)
		{
			diff->string[count]=df+'0';
			//printf("count %d\n",count);
			count--;
		}
		borrow =0;
		
	}

	count=0;
	while(diff->string[count]=='0' && count <=diff->length)
		count++;
	if(count==0)
		return (void*)diff;
	else
	{
		int di = diff->length - count;
		int flag = 0;
		if(di==0)
		{
			di =1;
			flag =1;
		}
		dtype* ndiff = (dtype*)malloc(sizeof(dtype));
		ndiff->string = (char*)malloc(sizeof(char)*(di));
		ndiff->length = di;
		if(flag)
		{
			ndiff->string[0]='0';
			ndiff->length = 1;
			return (void*)ndiff; //if there is 999 - 999 = 0 . returns only one zero.
		}
		for(int k=0;k<ndiff->length;k++)
		{
			ndiff->string[k]=diff->string[count];
			count++;
		}
		ndiff->string = (char*)realloc(ndiff->string,ndiff->length+1);
		ndiff->string[ndiff->length]='\0';
		intal_destroy(diff);
		//printf("PRINT --- %s\n",ndiff->string);
		return (void*)ndiff;
	}
}


int intal_compare(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2 == NULL)
		return -2;                      //return -2 if one of the input is NULL
	dtype* a = (dtype*)intal1;
	dtype* b = (dtype*)intal2;

	if(a->length < b->length)
		return -1;
	if(a->length > b->length)
		return 1;
	else
	{
		int i;
		for(i=0;i<a->length;i++)
		{
			if(a->string[i]>b->string[i])
				return 1;
			else if(b->string[i]>a->string[i])
				return -1;
		}
		if(i==a->length)
			return 0;
	}
}

void intal_destroy(void* intal)
{
	if(intal==NULL)
	{
		return;
	}

	dtype* x = (dtype*)intal;
	//printf(" FREE ADDRESS %ld\n",&x->string);
	free(x->string);
	free(x);
}

char* intal2str(void* intal)
{
	if(intal==NULL)
	{
		char* nan = malloc(sizeof(char)*3);
		strcpy(nan,"NaN");
		return nan;          //printing NaN if it is a NULL
	}


	dtype* x =(dtype*)intal;
	//printf(" STRING ADDRESS %ld ",&x->string);
	x->string = (char*)realloc(x->string,(x->length+1));
	x->string[x->length]='\0';         //adding null character at the end.
	char* sendback = (char*)malloc(sizeof(char)*x->length+1);
	strcpy(sendback,x->string);
	return sendback;
}


void* intal_divide(void* intal1, void* intal2) //(intal1 / intal2)
{
	if(intal1==NULL||intal2==NULL)
		return NULL;
	dtype* n1 = (dtype*)intal1;
	dtype* n2 = (dtype*)intal2;

	if(n2->string[0]=='0')
	{
		return NULL;              //divide by zero ERROR;
	}
	if(n2->string[0]=='1' && n2->length==1)
	{
		void* new_cpy = intal_create(((dtype*)intal1)->string);
		return new_cpy;
	}

	int x = intal_compare(intal1,intal2);
	if(x==0)
	{
		dtype* div = (dtype*)malloc(sizeof(dtype));
		div->string = (char*)malloc(sizeof(char)*(1));
		div->length = 1;
		div->string[0]='1';
		//printf("Quotient is : %s\n",div->string);
		return (void*)div;
	}
	else if(x<0)
	{
		return intal_create("0"); //returns a single zero
	}
	
	dtype* quotient = (dtype*)malloc(sizeof(dtype));
	quotient->string=(char*)malloc(sizeof(char)*1000000);
	quotient->length=0;

	int i=0;//global i in this function

	dtype* try = (dtype*)malloc(sizeof(dtype));
	try->string = (char*)malloc(sizeof(char)*(n2->length));
	try->length = n2->length;
	int j;
	for(j=0;j<n2->length;j++)
		{
			try->string[j]=n1->string[j];   //creating a number of size equal to the divisor.
		}
	i=j;
	if(i==n1->length)
	{
		i=n1->length-1;
	}

	// printf(" length n2 =%d",n2->length);
	// printf("try ---> %s\n",try->string);
	while(i<n1->length)
	{
		void* ref;
		void* nref;
		
		if(intal_compare((void*)try,(void*)n2) < 0) // more digits required in order to divide.
		{
			while(1)
			{
				if(i>=n1->length)
				{
					//printf("Escaping!!!");
					return (void*)quotient;
				}
				if(try->string[0]=='0')
				{
					free (try->string);
					try->string =(char*)malloc(sizeof(char)*1);
					try->length=1;
					try->string[0]=n1->string[i];
				}
				else
				{
					try->string = (char*)realloc(try->string,try->length+1);
					try->string[try->length]=n1->string[i]; //i is global
					//try->string[try->length+1]='\0';
					try->length++;
				}


				if(intal_compare((void*)try,(void*)n2) < 0)
				{
					quotient->string[quotient->length++]='0';
					i++;
				}
				else
					break;

			}
			//printf("### %s \n",try->string);
			//printf("### %d \n",try->length);
			i++;
		}

		int quot=0;//counter for quotient
		//repeated subtraction of n2 from try
		ref = intal_diff((void*)try,(void*)n2);
		free(try->string);
		// dtype* sum5 = (dtype*)ref;
		// sum5->string = (char*)realloc(sum5->string,sum5->length+1);
		// sum5->string[sum5->length]='\0';
		//printf("%s\n",sum5->string);
		while(1)
		{
			quot++;
			if(intal_compare(ref,(void*)n2)<0)
			{
				dtype* sum = (dtype*)ref;
				if(sum->string==NULL)  //string  was becoming null for some of the cases.
				{
					sum->string = (char*)malloc(sizeof(char)*1);
					try->string[0]='0';
				}
				//printf("Escaping it through %s\n",sum->string);
				break;
			}
			nref = intal_diff(ref,(void*)n2);//intal_diff returns a new copy everytime .
			dtype* sum = (dtype*)nref;       // The new copy has to be deleted in the next iteration 
												 // else memory leak will take place.
			// sum->string = (char*)realloc(sum->string,sum->length+1); 
			// sum->string[sum->length]='\0';
			//ref = intal_diff(ref,(void*)n2);
			intal_destroy(ref);
			ref = nref;
			nref=NULL;
		}
			
			quotient->string[quotient->length++]=quot+'0';
			dtype* sum = (dtype*)ref;
			// sum->string = (char*)realloc(sum->string,sum->length+1); 
			// sum->string[sum->length]='\0';

			if(sum->length==0)
				sum->length=1;

			try->string = (char*)malloc(sizeof(char)*sum->length);
			//strcpy(try->string,sum->string);  strcpy gave error for single denominator vales.(gdb said strcpy was unaligned)
			for(int m=0;m<sum->length;m++)
			{
				try->string[m]=sum->string[m];
			}
			try->length = sum->length;

			if(try->string==NULL)
			{
				try->string = (char*)malloc(sizeof(char)*1);
				try->string[0]='0';
			}
			//printf("-------%s\n",try->string);
			//printf(">>>> %s\n",quotient->string);
			intal_destroy(ref);

			if(n1->length == n2->length)
			{
				break;
			}
	}
	//printf("Quotient is : %s",quotient);
	quotient->string = (char*)realloc(quotient->string,sizeof(char)*(strlen(quotient->string)));
	//printf(" quotient --> %s",quotient->string);
	return (void*)quotient;		
}


/* Implemented Russian peasant method of multiplying; however it took longer than brute force.
void* russian_peasant_multiply(void* intal1,void* intal2)
{
	void* first = intal_create(((dtype*)intal1)->string);
	void* second = intal_create(((dtype*)intal2)->string);
	void* total=intal_create("0");
	void* two = intal_create("2");
	while(!(((dtype*)first)->string[0]=='1' && ((dtype*)first)->length ==1))
	{
		if(is_odd(first))
		{
			void* newtotal = intal_add(total,second);
			intal_destroy(total);
			total = newtotal;
			newtotal = NULL;
		}
		void* bytwo = intal_divide(first,two);
		intal_destroy(first);
		first = bytwo;
		bytwo = NULL;
		void* twice = intal_add(second,second);
		intal_destroy(second);
		second = twice;
		twice = NULL;
	}

	void* newtotal = intal_add(total,second);
	intal_destroy(total);
	intal_destroy(two);
	return newtotal;
}
*/

void* intal_multiply(void* intal1, void* intal2) //BRUTE FORCE 
{
	if(intal1==NULL || intal2==NULL)
		return NULL;
	dtype* n1 = (dtype*)intal1;
	dtype* n2 = (dtype*)intal2;

	if(n2->string[0]=='0' || n1->string[0]=='0')
	{
		return intal_create("0");              //multiplication by zero
	}
	if(n2->string[0]=='1' && n2->length==1)
	{
		void* new_cpy = intal_create(((dtype*)intal1)->string);
		return new_cpy;
	}
	else if(n1->string[0]=='1' && n1->length==1)
	{
		void* new_cpy = intal_create(((dtype*)intal2)->string);
		return new_cpy;
	}
	if(n1->length < n2->length) //Ensuring intal1 is larger in length than intal2
		{
			dtype* temp = n1;
			n1=n2;
			n2=temp;
		}
	dtype* mult = (dtype*)malloc(sizeof(dtype));
	mult->string = (char*)malloc(sizeof(char)*(n1->length+n2->length));
	mult->length = n1->length+n2->length;

	for(int m=0;m<mult->length;m++)
	{
		mult->string[m]='\0';
	}
	
	int i,j;
	int carry=0;
	int shift_val=0;
	for(i=n1->length-1;i>=0;i--)
	{
		carry=0;
		int k=shift_val;
		for(j=n2->length-1;j>=0;j--)
		{
			int m = (n1->string[i]-'0')*(n2->string[j]-'0')+carry;
			if(mult->string[k])
				m = m + (mult->string[k]-'0');
			mult->string[k++] =m%10 + '0';
			carry = m/10;
		}
		if(carry>0)
			mult->string[k]=carry+'0';
		shift_val++;
	}
	int len = strlen(mult->string);
	if(mult->string[len-1]=='0')
		{
			dtype* newmult = (dtype*)malloc(sizeof(dtype));
			newmult->string = (char*)malloc(sizeof(char)*1);
			newmult->length = 1;
			newmult->string[0]='0';
			free(mult->string);
			free(mult);
			return (void*)newmult;
		}
	if(len < mult->length)
	{
		//printf("Entered\n");
		dtype* newmult = (dtype*)malloc(sizeof(dtype));
		newmult->string = (char*)malloc(sizeof(char)*(len));
		newmult->length = len;
		int count = mult->length-2;
		for(int x=0;x<newmult->length;x++)
		{
			newmult->string[x]=mult->string[count];
			count--;
		}
		free(mult->string);
		free(mult);
		return (void*)newmult;
	}
	else
	{
		dtype* newmult = (dtype*)malloc(sizeof(dtype));
		newmult->string = (char*)malloc(sizeof(char)*(n1->length+n2->length));
		newmult->length = n1->length+n2->length;
		int count = mult->length-1;
		for(int x=0;x<newmult->length;x++)
		{
			newmult->string[x]=mult->string[count];
			count--;
		}
		free(mult->string);
		free(mult);
		return (void*)newmult;

	}
}

void* intal_pow(void* intal1, void* intal2) //intal1^intal2
{
	if(intal1==NULL || intal2==NULL)
		return NULL;
	// else if(((dtype*)intal1)->string[0]=='0' && ((dtype*)intal2)->string[0]=='0')
	// {
	// 	return NULL; // when it is 0^0
	// }
	else if(((dtype*)intal1)->string[0]=='0') //when base number is a zero.
	{
		void* power = intal_create("0");
		return power;
	}
	else if(((dtype*)intal2)->string[0]=='0') // when exponent is a zero.
	{
		void* power = intal_create("1");
		return power;
	}
	
	else if(((dtype*)intal1)->string[0]=='1' && (((dtype*)intal1)->length==1)) //when base number is a one.
	{
		void* power = intal_create("1");
		return power;
	}
	else if (((dtype*)intal2)->string[0]=='1' && (((dtype*)intal2)->length==1)) //when exponent is a one.
	{
		void* new_cpy = intal_create(((dtype*)intal1)->string);
		return new_cpy;
	}
	void* temp = intal_create(((dtype*)intal2)->string);
	void* two = intal_create("2");
	void* power = intal_pow_recursive(intal1 ,temp,two);
	intal_destroy(temp);
	intal_destroy(two);
	return power;
}

static void* intal_pow_recursive(void* intal1 ,void* intal2,void* two)
{
	if(((dtype*)intal2)->string[0]=='0')
	{
		return intal_create("1");
	}

	void* div = intal_divide(intal2,two); //intal_divide automatically returns the floor.
	void* p = intal_pow_recursive(intal1 ,div ,two);
	intal_destroy(div);
	void* newp= intal_multiply(p,p);
	intal_destroy(p);
	if(is_odd(intal2))
	{
		void* newnewp= intal_multiply(newp,intal1);
		intal_destroy(newp);
		return newnewp;
	}
	return newp;
}

static int is_odd(void* number)
{
	dtype* num = (dtype*)number;
	if((num->string[num->length-1]-'0')%2==0)
		return 0;
	else 
		return 1;
}

/*
void* intal_multiply(void* intal1, void* intal2)
{
	dtype* str1 = (dtype*)intal1;
	dtype* str2 = (dtype*)intal2;
	printf("%d %d\n",str1->length,str2->length);
	if(str1->length > str2->length)
		{
			printf("Switched\n");
			return intal_multiply(intal2,intal1);
		}
	if(str2->length < 2*(str1->length))
	{
		printf("Entered");
		int diff = str2->length - str1->length;
		dtype* newintal1 = (dtype*)malloc(sizeof(dtype));
		newintal1->string = (char*)malloc(sizeof(char)*(str2->length));
		newintal1->length = str2->length;

		for(int i=0;i<diff;i++)
		{
			newintal1->string[i]='0';
		}
		strcat(newintal1->string,str1->string);
		printf("%s\n",newintal1->string);
		//karatsuba(newintal1,str2);
	}

	dtype* c=(dtype*)malloc(sizeof(dtype));
	c->string = (char*)malloc(sizeof(char)*str2->length);
	c->length = str2->length;
	int i;
	for(i=0;i<str2->length-str1->length;i++)
	{
		c->string[i]=str2->string[i];
	}
	int j=i;
	while(i<str2->length)
	{
		c->string[i]='0';
		i++;
	}
	dtype* d=(dtype*)malloc(sizeof(dtype));
	d->string = (char*)malloc(sizeof(char)*str1->length);
	d->length = str1->length;

	for(i=0;i<str1->length;i++)
	{
		
		d->string[i]=str2->string[j];
		j++;
	}
	dtype* e = karatsuba(str1,d);
	dtype* f = intal_multiply(str1,c);

	f->string=(char*)realloc(f->string,sizeof(char)*(f->length+str1->length));
	for(int i=f->length;i<f->length+str1->length;i++)
	{
		f->string[i]='0';
	}
	f->length = f->length+str1->length;
	return intal_add(e,f);
}

void* karatsuba(void* a,void* b)
{
	if(a->length == 1)
	{
		int temp = (a->string[0]-'0')*(b->string[0]-'0');
		if(temp/10)
		{
			dtype* new=(dtype*)malloc(sizeof(dtype));
			new->string = (char*)malloc(sizeof(char)*2);
			new->length = 2;
			sprintf(new->string,"%d",temp);
		}
		else
		{
			dtype* new=(dtype*)malloc(sizeof(dtype));
			new->string = (char*)malloc(sizeof(char)*1);
			new->length = 1;
			sprintf(new->string,"%d",temp);
		}
		return (void*)new;
	}

	int m,n;
	n = a->length;
	m=a->length/2;


	dtype a1 = *((dtype*)a);
	a1.length = m;

	dtype a2=*((dtype*)a);
	a2.string = (a->string) + m;
	a2.length = n-m;

	dtype b1 = *((dtype*)b);
	b1.length = m;

	dtype b2=*((dtype*)(b));
	b2.string = (b->string) + m;
	b2.length = n-m;

	void* p1 = karatsuba((void*)a1,(void*)b1);
	void* p2 = karatsuba((void*)a2,(void*)b2);

	void* p3 = karatsuba(intal_add((void*)&a1,(void*)&a2),intal_add((void*)&b1,(void*)&b2));

}
*/




