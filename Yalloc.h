#include<stdio.h>
#include<stddef.h>
//this is the metadata block structure
struct block
{
	size_t size;
	struct block *next;
	struct block *prev;
};

char *memory;

struct block *freelist=NULL;
struct block *allocated=NULL;
void init()
{
	freelist->next=NULL;
	freelist->prev=NULL;
	freelist->size=40000-sizeof(struct block);
}


//IF size of th block is bigger than required size than this function will help to spilit the blocks & it will rearrange the freelist in right order.(according to Best Fit algorithm and freelist  is arranged in the ascending order.)
void spilit(struct block * curr,ssize_t size)
{
	struct block *curr2=allocated,*rotate;
	struct block *temp2=(void*)((void*)curr+sizeof(struct block)+size);
	temp2->size=(curr->size)-(sizeof(struct block) + size);
	temp2->next=NULL;
	temp2->prev=NULL;

	if(curr->next!=NULL)    {
		curr->next->prev=curr->prev; rotate=curr->next;}

	if(curr->prev!=NULL)    {
		curr->prev->next=curr->next;  rotate=curr->prev;}


	if(curr->prev==NULL && curr->next==NULL)
	{
		freelist=temp2;
		rotate=temp2;
	}


	if(rotate->size<temp2->size)
	{
		while(rotate->size<temp2->size && rotate->next!=NULL && rotate)

		{rotate=rotate->next;}

		if(rotate->next==NULL && rotate->size<temp2->size)
		{
			rotate->next=temp2;
			temp2->prev=rotate;
		}

		else{
			temp2->next=rotate;
			temp2->prev=rotate->prev;
			rotate->prev->next=temp2;
			rotate->prev=temp2;
		}
	}

	if(rotate->size>temp2->size)
	{
		while(rotate->size>temp2->size && rotate->prev!=NULL && rotate)
		{rotate=rotate->prev;}

		if(rotate->prev==NULL && rotate->size>temp2->size)
		{
			temp2->next=rotate;
			rotate->prev=temp2;
		}
		else
		{
			temp2->next=rotate;
			temp2->prev=rotate->prev;
			rotate->prev->next=temp2;
			rotate->prev=temp2;
		}
	}

	if(curr2==NULL)
	{
		allocated=curr;
		allocated->next=NULL;
		allocated->prev=NULL;
		allocated->size=size;
	}
	else
	{
		while(curr2->next!=NULL)
		{curr2=curr2->next;}
		curr2->next=curr;
		curr->next=NULL;
		curr->prev=curr2;
		curr->size=size;
	}
}



void *Yalloc(ssize_t size)
{
	void *result;
	struct block *curr=freelist;
	struct block *prev2,*curr2=allocated;
	if(!(freelist->size))
	{
		init(freelist);
	}

	while(((curr->size)< size) && curr->next!=NULL){
		curr=curr->next;}

	if(curr->size==size)
	{
		if(curr2==NULL)
		{
			allocated=curr;
			allocated->next=NULL;
			allocated->prev=NULL;
			allocated->size=size;
			result=(void*)++curr;
			return result;
		}
		else{
			while(curr2->next!=NULL)
			{
				curr2=curr2->next;
			}	

			if(curr->prev==NULL && curr->next==NULL){freelist=NULL;}
			if(curr->next!=NULL) {
				curr->next->prev=curr->prev;}
			if(curr->prev!=NULL)  {
				curr->prev->next=curr->next;}

			curr2->next=curr; 
			curr->prev=curr2 ; 
			curr->next=NULL;
			result=(void*)++curr; 
			return result;
		}
	}
	if(curr->size > (size+sizeof(struct block)))
	{
		spilit(curr,size);
		result=(void*)++curr;
		return result;
	}
	else
	{
		result=NULL;
		printf("insufficient memory");
		return result;
	}

}


//Merge the two blocks by removing medata block in the middle from freelist 
void merge()
{
	struct block *curr=freelist;
	while(curr && curr->next!=NULL)
	{
		if( (void*)( (void*)curr + curr->size + (sizeof(struct block)) ) == (void*)curr->next )
		{
			curr->size += curr->next->size + sizeof(struct block);
			curr->next=curr->next->next;

			if(curr->next!=NULL)
			{
				curr->next->prev=curr;
			}
		}
		else
			curr=curr->next;
	}


}

//make block free add it to freelist at right place.(according to Best Fit algorithm and freelist  is arranged in the ascending order.)

void *Yfree(void *ptr)
{
	struct block *curr=NULL,*pr=NULL;
	struct block *curr2=freelist;
	if( ((void*)memory < ptr) && (ptr<(void*)(memory+40000)) )
	{
		curr=(void*)ptr;
		--curr;
		pr=curr->prev;
		if(pr!=NULL)
		{
			pr->next=curr->next;
		}

		if(curr->next!=NULL)
		{
			curr->next->prev=curr->prev;
		}
		if(curr->next==NULL && curr->prev==NULL)
		{
			allocated=NULL;
		}
		while(curr2 && curr2->next!=NULL && curr->size > (curr2->size))
		{
			curr2=curr2->next;
		}

		if(curr->size==curr2->size){
			if((void*)curr2 < (void*)curr){
				curr->next=curr2->next;
				curr2->next=curr;
				curr->prev=curr2;

				if(curr->next!=NULL){
					curr->next->prev=curr;
				}
			}
			if((void*)curr2 > (void*)curr){
				curr->next=curr2;
				if(curr2->prev!=NULL){
					curr->prev=curr2->prev;
					curr2->prev->next=curr;
				}
				else{
					curr->prev=NULL;}
				curr2->prev=curr;
				freelist=curr;
			}
			merge();
			return 0;
		}

		if(curr->size>curr2->size && curr2->next==NULL)
		{
			curr2->next=curr;
			curr->prev=curr2;
			merge();
		}

		else
		{
			curr->prev=curr2->prev; 
			curr->next=curr2; 																															
			if(curr2->prev==NULL)
			{
				freelist=curr; 
			}
			else {
				curr2->prev->next=curr; }
			curr2->prev=curr;
			merge();
			return 0;
		}

	}
	else
		printf("error\n");

}

