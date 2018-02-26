#include "core/data/linked_list.h"


namespace Data {
   // -------------------------------------------------------------------------
   bool isEmpty(SingleListHead list) { return (list.head == nullptr); }
   bool isEmpty(DoubleListHead list) { return (list.head == nullptr); }
   bool isEmpty(SingleListTail list) { return (list.tail == nullptr); }
   bool isEmpty(DoubleListTail list) { return (list.tail == nullptr); }

   // ------------------------------------------------------------------------
   SingleListNode* head(SingleListHead list) { return list.head; }
   DoubleListNode* head(DoubleListHead list) { return list.head; }
   SingleListNode* tail(SingleListTail list) { return list.tail; }
   DoubleListNode* tail(DoubleListTail list) { return list.tail; }

	// ------------------------------------------------------------------------
	SingleListHead next(SingleListHead list)
	{
		return SingleListHead{ list.head->next };
	}

   // ------------------------------------------------------------------------
   DoubleListHead next(DoubleListHead list)
   {
      return DoubleListHead{ list.head->next };
   }

   // -------------------------------------------------------------------------
	SingleListHead addToHead(SingleListHead list, SingleListNode* node)
	{
		node->next = list.head;
		return SingleListHead{ node };
	}

   // -------------------------------------------------------------------------
   DoubleListHead addToHead(DoubleListHead list, DoubleListNode* node)
   {
      if (!isEmpty(list)) {
         return addToNonEmptyHead(list, node);
      } else {
         node->prev = nullptr;
         node->next = nullptr;
         return DoubleListHead{ node };
      }
      
   }

	// -------------------------------------------------------------------------
	SingleListTail addToTail(SingleListTail list, SingleListNode* node)
	{
		if (!isEmpty(list)) {
			return addToNonEmptyTail(list, node);
		}
		else {
         node->next = nullptr;
			return SingleListTail{ node };
		}
	}

   // -------------------------------------------------------------------------
   DoubleListHead addToNonEmptyHead(DoubleListHead list, DoubleListNode* node)
   {
      node->prev = nullptr;
      node->next = list.head;
      list.head->prev = node;
      return DoubleListHead{ node };
   }

	// -------------------------------------------------------------------------
	SingleListTail addToNonEmptyTail(SingleListTail list, SingleListNode* node)
	{
		node->next = nullptr;
		list.tail->next = node;
		return SingleListTail{ node };
	}

	// -------------------------------------------------------------------------
	SingleListHead removeHead(SingleListHead list)
	{
		if (isEmpty(list)) return list;
		return SingleListHead{ list.head->next };
	}

   // -------------------------------------------------------------------------
   DoubleListHead remove(DoubleListHead list, DoubleListNode* node)
   {
      if (!node->prev && !node->next) {
         return DoubleListHead{ nullptr };
      }

      if (!node->prev) {
         node->next->prev = nullptr;
         return DoubleListHead{ node->next };
      }
      if (!node->next) {
         node->prev->next = nullptr;
         return DoubleListHead{ node->prev };
      }

      node->prev->next = node->next;
      node->next->prev = node->prev;
      return list;
   }

	// -------------------------------------------------------------------------
	SingleListHeadCollection iterate(SingleListHead list)
	{
		return SingleListHeadCollection(list);
	}

} // namespace Data
