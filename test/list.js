const assert = require('power-assert');
const LinkedList = require('../index').LinkedList;

describe('About new LinkedList', () => {
    it('new list is empty', () => {
        var list = new LinkedList();
        assert(list.length === 0);
    });
    it('a list can add and remove items', () => {
        var list = new LinkedList();
        list.addFirst('a');
        list.addFirst('b');
        list.addFirst('c');
        assert(list.length === 3);
        assert(list.removeFirst() === 'c');
        assert(list.removeFirst() === 'b');
        assert(list.removeFirst() === 'a');
        assert(list.length === 0);
        assert(list.removeFirst() === undefined);
        assert(list.length === 0);
    });
});

