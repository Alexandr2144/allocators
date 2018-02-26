#include "data/linked_list.h"


namespace Data {
	// ------------------------------------------------------------------------
	SingleListHead next(SingleListHead list)
	{
		return SingleListHead{ list.head->next };
	}

	// ------------------------------------------------------------------------
	SingleListNode* head(SingleListHead list)
	{
		return list.head;
	}

	// ------------------------------------------------------------------------
	SingleListNode* tail(SingleListTail list)
	{
		return list.tail;
	}

    // ------------------------------------------------------------------------
	SingleListHead addToHead(SingleListHead list, SingleListNode* node)
	{
		node->next = list.head;
		return SingleListHead{ node };
	}

	// ------------------------------------------------------------------------
	SingleListTail addToTail(SingleListTail list, SingleListNode* node)
	{
		if (!isEmpty(list)) {
			return addToNonEmptyTail(list, node);
		}
		else {
			return SingleListTail{ node };
		}
	}

	// ------------------------------------------------------------------------
	SingleListTail addToNonEmptyTail(SingleListTail list, SingleListNode* node)
	{
		node->next = nullptr;
		list.tail->next = node;
		return SingleListTail{ node };
	}

	// ------------------------------------------------------------------------
	SingleListHead removeHead(SingleListHead list)
	{
		if (isEmpty(list)) return list;
		return SingleListHead{ list.head->next };
	}

	// ------------------------------------------------------------------------
	bool isEmpty(SingleListHead list)
	{
		return (list.head == nullptr);
	}

	// ------------------------------------------------------------------------
	bool isEmpty(SingleListTail list)
	{
		return (list.tail == nullptr);
	}

	// ------------------------------------------------------------------------
	SingleListHeadCollection iterate(SingleListHead list)
	{
		return SingleListHeadCollection(list);
	}

} // namespace Data
