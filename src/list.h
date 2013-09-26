#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <node.h>

/*
 * A implementation of linked list.
 */

/**
 * An Item of linked list.
 */
struct Entry {
	Entry();
	~Entry();
	
	Entry* next;
	Entry* prev;
	v8::Persistent<v8::Value> value;
};

/**
 * Linked list
 */
class LinkedList : public node::ObjectWrap {
	public:

		static void Init(v8::Handle<v8::Object> exports);

	private:

		LinkedList();
		~LinkedList();

		void Push(const v8::Handle<v8::Value>& value);

		/**
		 * Invoked when LinkedList constructor is called on JS.
		 */
		static v8::Handle<v8::Value> New(const v8::Arguments& args);
		
		/**
		 * Invoked when LinkedList#addLast method is called on JS.
		 */
		static v8::Handle<v8::Value> AddLast(const v8::Arguments& args);
		
		/**
		 * Invoked when LinkedList#addFirst method is called on JS.
		 */
		static v8::Handle<v8::Value> AddFirst(const v8::Arguments& args);
		
		/**
		 * Invoked when LinkedList#removeLast method is called on JS.
		 */
		static v8::Handle<v8::Value> RemoveLast(const v8::Arguments& args);
		
		/**
		 * Invoked when LinkedList#removeFirst method is called on JS.
		 */
		static v8::Handle<v8::Value> RemoveFirst(const v8::Arguments& args);
		
		/**
		 * Invoked when you read length property on JS.
		 */
		static v8::Handle<v8::Value> GetLength(v8::Local<v8::String> name,  const v8::AccessorInfo& info);
		
		/**
		 * Invoked when you read first property on JS.
		 */
		static v8::Handle<v8::Value> GetFirst(v8::Local<v8::String> name,  const v8::AccessorInfo& info);
		
		/**
		 * Invoked when you read last property on JS.
		 */
		static v8::Handle<v8::Value> GetLast(v8::Local<v8::String> name,  const v8::AccessorInfo& info);

		/**
		 * forEach
		 */
		static v8::Handle<v8::Value> ForEach(const v8::Arguments& args);
		
		/** First Item of the list */
		Entry* head;

		/** Last Item of the list */
		Entry* tail;

		/** Size of the list */
		int length;
};

#endif
