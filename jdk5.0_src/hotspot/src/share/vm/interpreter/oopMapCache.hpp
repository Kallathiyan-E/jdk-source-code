#ifdef USE_PRAGMA_IDENT_HDR
#pragma ident "@(#)oopMapCache.hpp	1.49 04/03/29 14:12:28 JVM"
#endif
/*
 * Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
 * SUN PROPRIETARY/CONFIDENTIAL.  Use is subject to license terms.
 */

// A Cache for storing (method, bci) -> oopMap.
// The memory management system uses the cache when locating object 
// references in an interpreted frame.
//
// OopMapCache's are allocated lazily per instanceKlass.

// The oopMap (InterpreterOopMap) is stored as a bit mask. If the 
// bit_mask can fit into two words it is stored in
// the _bit_mask array, otherwise it is allocated on the heap.
// For OopMapCacheEntry the bit_mask is allocated in the C heap 
// because these entries persist between garbage collections.
// For InterpreterOopMap the bit_mask is allocated in
// a resource area for better performance.  InterpreterOopMap
// should only be created and deleted during same garbage collection.
//
// If ENABBLE_ZAP_DEAD_LOCALS is defined, two bits are used 
// per entry instead of one. In all cases,
// the first bit is set to indicate oops as opposed to other 
// values. If the second bit is available,
// it is set for dead values. We get the following encoding:
//
// 00 live value
// 01 live oop
// 10 dead value
// 11 <unused>                                   (we cannot distinguish between dead oops or values with the current oop map generator)


class OffsetClosure  {
 public:
  virtual void offset_do(int offset) = 0;
};


class InterpreterOopMap: ResourceObj {
  friend class OopMapCache;

 public:
  enum {
    N                = 2,                // the number of words reserved 
					 // for inlined mask storage
    small_mask_limit = N * BitsPerWord,  // the maximum number of bits 
					 // available for small masks,
					 // small_mask_limit can be set to 0 
					 // for testing bit_mask allocation

#ifdef ENABLE_ZAP_DEAD_LOCALS
    bits_per_entry   = 2,
    dead_bit_number  = 1,
#else
    bits_per_entry   = 1,
#endif
    oop_bit_number   = 0
  };

 private:
  methodOop      _method;         // the method for which the mask is valid
  unsigned short _bci;            // the bci    for which the mask is valid
  int            _mask_size;      // the mask size in bits
  int            _expression_stack_size; // the size of the expression stack in slots

 protected:
  intptr_t       _bit_mask[N];    // the bit mask if 
				  // mask_size <= small_mask_limit, 
				  // ptr to bit mask otherwise
				  // "protected" so that sub classes can 
				  // access it without using trickery in 
				  // methd bit_mask().
#ifdef ASSERT
  bool _resource_allocate_bit_mask;
#endif

  // access methods
  methodOop      method() const                  { return _method; }
  void		 set_method(methodOop v)	 { _method = v; }
  int            bci() const                     { return _bci; }
  void		 set_bci(int v)			 { _bci = v; }
  int            mask_size() const               { return _mask_size; }
  void		 set_mask_size(int v)		 { _mask_size = v; }
  int            number_of_entries() const       { return mask_size() / bits_per_entry; }
  // Test bit mask size and return either the in-line bit mask or allocated
  // bit mask.
  uintptr_t*  bit_mask()                         { return (uintptr_t*)(mask_size() <= small_mask_limit ? (intptr_t)_bit_mask : _bit_mask[0]); }

  // return the word size of_bit_mask.  mask_size() <= 4 * MAX_USHORT
  size_t mask_word_size() { 
    return (mask_size() + BitsPerWord - 1) / BitsPerWord; 
  } 

  uintptr_t entry_at(int offset)            { int i = offset * bits_per_entry; return bit_mask()[i / BitsPerWord] >> (i % BitsPerWord); }

  void set_expression_stack_size(int sz)    { _expression_stack_size = sz; }

#ifdef ENABLE_ZAP_DEAD_LOCALS
  bool is_dead(int offset)                       { return (entry_at(offset) & (1 << dead_bit_number)) != 0; }
#endif

  // Lookup
  bool match(methodHandle method, int bci)       { return _method == method() && _bci == bci; }
  bool is_empty();

  // Initialization
  void initialize();  

 public:
  InterpreterOopMap();
  ~InterpreterOopMap();

  // Copy the OopMapCacheEntry in parameter "from" into this
  // InterpreterOopMap.  If the _bit_mask[0] in "from" points to
  // allocated space (i.e., the bit mask was to large to hold
  // in-line), allocate the space from a Resource area.
  void resource_copy(OopMapCacheEntry* from);

  void iterate_oop(OffsetClosure* oop_closure);
  void oop_iterate(OopClosure * blk);
  void oop_iterate(OopClosure * blk, MemRegion mr);
  void verify();
  void print();  

  bool is_oop  (int offset)                      { return (entry_at(offset) & (1 << oop_bit_number )) != 0; }

  int expression_stack_size()                    { return _expression_stack_size; }

#ifdef ENABLE_ZAP_DEAD_LOCALS
  void iterate_all(OffsetClosure* oop_closure, OffsetClosure* value_closure, OffsetClosure* dead_closure);
#endif
};

class OopMapCache : public CHeapObj {
 private:
  enum { _size        = 32,     // Use fixed size for now
         _probe_depth = 3       // probe depth in case of collisions
  };                          

  OopMapCacheEntry* _array;

  unsigned int hash_value_for(methodHandle method, int bci);
  OopMapCacheEntry* entry_at(int i) const;

  Mutex _mut;

  void flush();

 public:
  OopMapCache();
  ~OopMapCache();                                // free up memory

  // Returns the oopMap for (method, bci) in parameter "entry".
  // Returns false if an oop map was not found.
  void lookup(methodHandle method, int bci, InterpreterOopMap* entry);

  // Compute an oop map without updating the cache or grabbing any locks (for debugging)
  static void compute_one_oop_map(methodHandle method, int bci, InterpreterOopMap* entry);

  // Helpers
  // Iterate over the entries in the cached OopMapCacheEntry's
  void oop_iterate(OopClosure *blk);
  void oop_iterate(OopClosure *blk, MemRegion mr);
  void verify();

  // Returns total no. of bytes allocated as part of OopMapCache's
  static long memory_usage()                     PRODUCT_RETURN0;
};

