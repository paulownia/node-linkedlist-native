#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <node.h>
#include <node_object_wrap.h>

/*
 * A implementation of linked list.
 */
namespace list {

	/**
	 * An Item of linked list.
	 */
	struct Entry {
		Entry();
		~Entry();

		Entry* next;
		Entry* prev;
		v8::UniquePersistent<v8::Value> value;
	};

	/**
	 * Linked list
	 */
	class LinkedList : public node::ObjectWrap {
		public:

			static void Init(v8::Local<v8::Object> exports);

		private:

			LinkedList();
			~LinkedList();

			void Push(v8::Isolate* isolate, v8::Local<v8::Value> value);

			/**
			 * Invoked when LinkedList constructor is called on JS.
			 */
			static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

			/**
			 * Invoked when LinkedList#addLast method is called on JS.
			 */
			static void AddLast(const v8::FunctionCallbackInfo<v8::Value>& args);

			/**
			 * Invoked when LinkedList#addFirst method is called on JS.
			 */
			static void AddFirst(const v8::FunctionCallbackInfo<v8::Value>& args);

			/**
			 * Invoked when LinkedList#removeLast method is called on JS.
			 */
			static void RemoveLast(const v8::FunctionCallbackInfo<v8::Value>& args);

			/**
			 * Invoked when LinkedList#removeFirst method is called on JS.
			 */
			static void RemoveFirst(const v8::FunctionCallbackInfo<v8::Value>& args);

			/**
			 * Invoked when you read length property on JS.
			 */
			static void GetLength(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& args);

			/**
			 * Invoked when you read first property on JS.
			 */
			static void GetFirst(v8::Local<v8::String> property,  const v8::PropertyCallbackInfo<v8::Value>& args);

			/**
			 * Invoked when you read last property on JS.
			 */
			static void GetLast(v8::Local<v8::String> property,  const v8::PropertyCallbackInfo<v8::Value>& args);

			/**
			 * forEach
			 */
			static void ForEach(const v8::FunctionCallbackInfo<v8::Value>& args);

			/** First Item of the list */
			Entry* head;

			/** Last Item of the list */
			Entry* tail;

			/** Size of the list */
			int length;
	};
}

#endif
