#pragma once
#ifndef DATA_LINKED_LIST_H
#define DATA_LINKED_LIST_H


namespace Data {
	struct SingleListNode { SingleListNode* next; };
	struct SingleListHead { SingleListNode* head; };
	struct SingleListTail { SingleListNode* tail; };

   struct DoubleListNode { DoubleListNode* prev; DoubleListNode* next; };
   struct DoubleListHead { DoubleListNode* head; };
   struct DoubleListTail { DoubleListNode* tail; };

	bool isEmpty(SingleListHead list);
	bool isEmpty(SingleListTail list);
   bool isEmpty(DoubleListHead list);

	SingleListHead next(SingleListHead list);
   DoubleListHead next(DoubleListHead list);

	SingleListNode* head(SingleListHead list);
   DoubleListNode* head(DoubleListHead list);

	SingleListNode* tail(SingleListTail list);
   DoubleListNode* tail(DoubleListTail list);

	SingleListHead addToHead(SingleListHead list, SingleListNode* node);
   DoubleListHead addToHead(DoubleListHead list, DoubleListNode* node);

	SingleListTail addToTail(SingleListTail list, SingleListNode* node);

   DoubleListHead addToNonEmptyHead(DoubleListHead list, DoubleListNode* node);
	SingleListTail addToNonEmptyTail(SingleListTail list, SingleListNode* node);

	SingleListHead removeHead(SingleListHead list);
   DoubleListHead remove(DoubleListHead list, DoubleListNode* node);


	class SingleListHeadCollection {
	public:
		SingleListHeadCollection(SingleListHead list)
			: m_list(list) {}

		SingleListNode* begin() { return m_list.head; }
		SingleListNode* end()   { return nullptr; }

	private:
		SingleListHead m_list;
	};

	SingleListHeadCollection iterate(SingleListHead list);

} // namespace Data


#endif // DATA_LINKED_LIST_H