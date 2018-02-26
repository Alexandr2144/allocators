#pragma once
#ifndef DATA_LINKED_LIST_H
#define DATA_LINKED_LIST_H


namespace Data {
	struct SingleListNode { SingleListNode* next; };
	struct SingleListHead { SingleListNode* head; };
	struct SingleListTail { SingleListNode* tail; };

	bool isEmpty(SingleListHead list);
	bool isEmpty(SingleListTail list);

	SingleListHead next(SingleListHead list);
	SingleListNode* head(SingleListHead list);
	SingleListNode* tail(SingleListTail list);

	SingleListHead addToHead(SingleListHead list, SingleListNode* node);
	SingleListTail addToTail(SingleListTail list, SingleListNode* node);

	SingleListTail addToNonEmptyTail(SingleListTail list, SingleListNode* node);

	SingleListHead removeHead(SingleListHead list);


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