
#include "ace/Log_Msg.h"
#include "ace/Containers.h"
#include "ace/Service_Config.h"
#include "DataElement.h"

typedef ACE_DLList<DataElement> MyList;

class ContainerTest
{
public:
	int list_run(void);
	int runBoundedStack(void);
	int runFixedStack(void);
	int runUnboundedStack(void);
	int runStackUnboundedQueue(void);
	int runHeapUnboundedQueue(void);
	
	int runArray(void);
	int runBoundedSet(void);
	int runUnBoundedSet(void);
	
	void displayList(MyList &list);
	void destroyList(MyList &list);
};

int ContainerTest::list_run(void)
{
	ACE_TRACE (ACE_TEXT("ListTest::run"));
	MyList list1;

	for (int i = 0; i < 100; i++)
	{
		DataElement *element;
		ACE_NEW_RETURN (element, DataElement (i), -1);
		list1.insert_tail (element);
	}

	this->displayList( list1);

	MyList list2;
	list2 = list1;
	this->displayList(list2);
	this->destroyList(list2);

	
	return 0;
}

void ContainerTest::displayList(MyList & list)
{
	ACE_TRACE(ACE_TEXT ("ListTest::displayList"));

	ACE_DEBUG ((LM_DEBUG, ACE_TEXT("Forward iteration\n")));
	ACE_DLList_Iterator<DataElement> iter (list);
	while(!iter.done())
	{
		ACE_DEBUG
			((LM_DEBUG, ACE_TEXT ("%d:"), iter.next()->getData()));
		iter++;
	}
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("\n")));

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("Reverse Iteration \n")));
	
	ACE_DLList_Reverse_Iterator<DataElement> riter (list);
	while(!riter.done())
	{
		ACE_DEBUG
			((LM_DEBUG, ACE_TEXT ("%d:"), riter.next()->getData()));
		riter++;
	}
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("\n")));
}

void ContainerTest::destroyList(MyList & list)
{
	
}

int ContainerTest::runBoundedStack(void)
{
	ACE_TRACE (ACE_TEXT ("StackExample::runBoundedStack"));
	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a bounded stack\n")));

	ACE_Bounded_Stack<DataElement> bstack1(100);

	{
		DataElement elem[100];

		for (int i = 0 ; i < 100; i ++)
		{
			elem[i].setData(i);
			bstack1.push(elem[i]);
		}
	}

	ACE_Bounded_Stack<DataElement> bstack2 (100);

	bstack2 = bstack1;
	for (int j = 0; j < 100; j ++)
	{
		DataElement elem;
		bstack2.pop(elem);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("%d:"), elem.getData()));
	}
	return 0;
}


int ContainerTest::runFixedStack(void)
{
	ACE_TRACE (ACE_TEXT ("StackExample::runFixedStack"));
	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a fixed stack\n")));

	ACE_Fixed_Stack<DataElement*, 100> bstack;

	for (int i = 0 ; i < 100; i ++)
	{
		DataElement* elem;
		ACE_NEW_RETURN(elem, DataElement(i), -1);
		
		bstack.push(elem);
	}

	for (int j = 0; j < 100; j ++)
	{
		DataElement* elem;
		bstack.pop(elem);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("%d:"), elem->getData()));
		delete elem;
	}
	return 0;
}


int ContainerTest::runUnboundedStack(void)
{
	ACE_TRACE (ACE_TEXT ("StackExample::runUnboundedStack"));
	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a unbounded stack\n")));

	ACE_Unbounded_Stack<DataElement*> ustack;

	for (int i = 0 ; i < 100; i ++)
	{
		DataElement* elem;
		ACE_NEW_RETURN(elem, DataElement(i), -1);
		
		ustack.push(elem);
	}

	ACE_Unbounded_Stack_Iterator<DataElement*> iter (ustack);
	for (iter.first();!iter.done();iter.advance())
	{
		DataElement** elem;
		iter.next(elem);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("%d:"), (*elem)->getData()));
		delete (*elem);
	}
	return 0;
}


int ContainerTest::runStackUnboundedQueue(void)
{
	ACE_TRACE (ACE_TEXT ("Example::runStackUnboundedQueue"));
	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a stack unbounded queue\n")));

	ACE_Unbounded_Queue<DataElement> queue;

	int i;
	for (i = 0 ; i < 10; i ++)
	{
		DataElement elem[10];
		elem[i].setData(9-i);
		queue.enqueue_head(elem[i]);
	}

	for (i = 0 ; i < 10; i ++)
	{
		DataElement elem[10];
		elem[i].setData(10 + i);
		queue.enqueue_tail(elem[i]);
	}
	ACE_Unbounded_Queue_Iterator<DataElement> iter (queue);
	for (iter.first();!iter.done();iter.advance())
	{
		DataElement* elem;
		iter.next(elem);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("%d:"), elem->getData()));
	}
	return 0;
}


int ContainerTest::runHeapUnboundedQueue(void)
{
	ACE_TRACE (ACE_TEXT ("Example::runHeapUnboundedQueue"));
	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a heap unbounded queue\n")));

	ACE_Unbounded_Queue<DataElement*> queue;

	int i;
	for (i = 0 ; i < 20; i ++)
	{
		DataElement *elem;
		ACE_NEW_RETURN(elem, DataElement(i), -1);
		queue.enqueue_head(elem);
	}

	ACE_Unbounded_Queue_Iterator<DataElement*> iter (queue);
	for (iter.first();!iter.done();iter.advance())
	{
		DataElement** elem;
		iter.next(elem);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("%d:"), (*elem)->getData()));
		delete (*elem);
	}
	return 0;
}

int ContainerTest::runArray(void)
{
	ACE_Array<DataElement*> arr(10);

	DataElement *elem = 0;
	for (int i = 0; i < 10; i++)
	{
		ACE_NEW_RETURN(elem, DataElement(i), -1);
		arr[i] = elem;
	}

	ACE_ASSERT(arr.set(elem, 11) == -1);
	ACE_ASSERT(arr.get(elem, 11) == -1);

	ACE_Array<DataElement*> copy = arr;
	ACE_ASSERT(copy == arr);

	ACE_Array<DataElement*>::ITERATOR iter(arr);
	while (!iter.done())
	{
		DataElement **elem;
		iter.next(elem);
		
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("%d:"), (*elem)->getData()));
		delete (*elem);

		iter.advance();
	}

	return 0;
}


int ContainerTest::runBoundedSet(void)
{
	ACE_TRACE (ACE_TEXT ("Example::runBoundedSet"));
	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a bounded set\n")));

	ACE_Bounded_Set<DataElement> bset(100);

	DataElement elem[100];
	for (int i = 0; i < 100; i++)
	{
		elem[i].setData(i);

		bset.insert(elem[i]);
		if (bset.insert(elem[i]) == -1)
		{
			ACE_DEBUG((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("insert set")));
		}
	}

	DataElement elem1(10), elem2(99);

	if (!bset.find(elem1) && !bset.find(elem2))
	{
		ACE_DEBUG((LM_INFO,
					ACE_TEXT("The elements %d and %d are ")
					ACE_TEXT("in the set!\n"),
					elem1.getData(), elem2.getData()));
	}

	for (int j = 0; j < 50; j++)
	{
		bset.remove(elem[j]);
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("%d:"), elem[j].getData()));
	}

	if ((bset.find(elem[0]) == -1) && (bset.find(elem[49]) == -1))
	{
		ACE_DEBUG((LM_INFO,
					ACE_TEXT("The elements %d and %d are ")
					ACE_TEXT("Not in the set!\n"),
					elem[0].getData(), elem[49].getData()));
	}

	return 0;
}

int ContainerTest::runUnBoundedSet(void)
{
	ACE_TRACE (ACE_TEXT ("Example::runUnBoundedSet"));
	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using a unbounded set\n")));

	ACE_Unbounded_Set<DataElement*> uset;

	for (int i = 0; i < 100; i++)
	{
		DataElement *elem;
		ACE_NEW_RETURN(elem, DataElement(i), -1);
		
		uset.insert(elem);
	}

	DataElement elem1(10), elem2(99);

	if (!uset.find(&elem1) && !uset.find(&elem2))
	{
		ACE_DEBUG((LM_INFO,
					ACE_TEXT("The elements %d and %d are ")
					ACE_TEXT("in the set!\n"),
					elem1.getData(), elem2.getData()));
	}

	ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Deleting the elements\n")));

	ACE_Unbounded_Set_Iterator<DataElement*> iter(uset);

	for (iter = uset.begin(); iter != uset.end(); iter++)
	{
		DataElement* elem = (*iter);
		ACE_DEBUG ((LM_DEBUG, ACE_TEXT("%d:"), elem->getData()));
		delete elem;
	}

	return 0;
}
int ACE_MAIN(int argc, ACE_TCHAR *argv[])
{
	ContainerTest myTest;
	//myTest.list_run();
	//myTest.runBoundedStack();
	//myTest.runFixedStack();
	//myTest.runUnboundedStack();
	//myTest.runStackUnboundedQueue();
	//myTest.runArray();
	//myTest.runBoundedSet();
	myTest.runUnBoundedSet();
	return 0;
}

