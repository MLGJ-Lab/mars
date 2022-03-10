#ifndef BOOST_THREAD_CONCURRENT_QUEUES_SYNC_QUEUE_HPP
#define BOOST_THREAD_CONCURRENT_QUEUES_SYNC_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2013-2014. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/thread for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include <third_party/boost/thread/detail/config.hpp>
#include <third_party/boost/thread/concurrent_queues/detail/sync_queue_base.hpp>
#include <third_party/boost/thread/concurrent_queues/queue_op_status.hpp>
#include <third_party/boost/thread/condition_variable.hpp>
#include <third_party/boost/thread/csbl/devector.hpp>
#include <third_party/boost/thread/detail/move.hpp>
#include <third_party/boost/thread/mutex.hpp>

#include <third_party/boost/throw_exception.hpp>
#include <third_party/boost/smart_ptr/shared_ptr.hpp>
#include <third_party/boost/smart_ptr/make_shared.hpp>

#include <third_party/boost/config/abi_prefix.hpp>

namespace mars_boost {} namespace boost = mars_boost; namespace mars_boost
{
namespace concurrent
{
  template <class ValueType, class Container = csbl::devector<ValueType> >
  class sync_queue
    : public detail::sync_queue_base<ValueType, Container >
  {
    typedef detail::sync_queue_base<ValueType, Container >  super;

  public:
    typedef ValueType value_type;
    //typedef typename super::value_type value_type; // fixme
    typedef typename super::underlying_queue_type underlying_queue_type;
    typedef typename super::size_type size_type;
    typedef typename super::op_status op_status;

    // Constructors/Assignment/Destructors
    BOOST_THREAD_NO_COPYABLE(sync_queue)
    inline sync_queue();
    //template <class Range>
    //inline explicit sync_queue(Range range);
    inline ~sync_queue();

    // Modifiers

    inline void push(const value_type& x);
    inline queue_op_status try_push(const value_type& x);
    inline queue_op_status nonblocking_push(const value_type& x);
    inline queue_op_status wait_push(const value_type& x);
    inline void push(BOOST_THREAD_RV_REF(value_type) x);
    inline queue_op_status try_push(BOOST_THREAD_RV_REF(value_type) x);
    inline queue_op_status nonblocking_push(BOOST_THREAD_RV_REF(value_type) x);
    inline queue_op_status wait_push(BOOST_THREAD_RV_REF(value_type) x);

    // Observers/Modifiers
    inline void pull(value_type&);
    // enable_if is_nothrow_copy_movable<value_type>
    inline value_type pull();

    inline queue_op_status try_pull(value_type&);
    inline queue_op_status nonblocking_pull(value_type&);
    inline queue_op_status wait_pull(ValueType& elem);

  private:

    inline queue_op_status try_pull(value_type& x, unique_lock<mutex>& lk);
    inline queue_op_status wait_pull(value_type& x, unique_lock<mutex>& lk);
    inline queue_op_status try_push(const value_type& x, unique_lock<mutex>& lk);
    inline queue_op_status wait_push(const value_type& x, unique_lock<mutex>& lk);
    inline queue_op_status try_push(BOOST_THREAD_RV_REF(value_type) x, unique_lock<mutex>& lk);
    inline queue_op_status wait_push(BOOST_THREAD_RV_REF(value_type) x, unique_lock<mutex>& lk);

    inline void pull(value_type& elem, unique_lock<mutex>& )
    {
      elem = mars_boost::move(super::data_.front());
      super::data_.pop_front();
    }
    inline value_type pull(unique_lock<mutex>& )
    {
      value_type e = mars_boost::move(super::data_.front());
      super::data_.pop_front();
      return mars_boost::move(e);
    }

    inline void push(const value_type& elem, unique_lock<mutex>& lk)
    {
      super::data_.push_back(elem);
      super::notify_not_empty_if_needed(lk);
    }

    inline void push(BOOST_THREAD_RV_REF(value_type) elem, unique_lock<mutex>& lk)
    {
      super::data_.push_back(mars_boost::move(elem));
      super::notify_not_empty_if_needed(lk);
    }
  };

  template <class ValueType, class Container>
  sync_queue<ValueType, Container>::sync_queue() :
    super()
  {
  }

//  template <class ValueType, class Container>
//  template <class Range>
//  explicit sync_queue<ValueType, Container>::sync_queue(Range range) :
//    data_(), closed_(false)
//  {
//    try
//    {
//      typedef typename Range::iterator iterator_t;
//      iterator_t first = mars_boost::begin(range);
//      iterator_t end = mars_boost::end(range);
//      for (iterator_t cur = first; cur != end; ++cur)
//      {
//        data_.push(mars_boost::move(*cur));;
//      }
//      notify_not_empty_if_needed(lk);
//    }
//    catch (...)
//    {
//      delete[] data_;
//    }
//  }

  template <class ValueType, class Container>
  sync_queue<ValueType, Container>::~sync_queue()
  {
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::try_pull(ValueType& elem, unique_lock<mutex>& lk)
  {
    if (super::empty(lk))
    {
      if (super::closed(lk)) return queue_op_status::closed;
      return queue_op_status::empty;
    }
    pull(elem, lk);
    return queue_op_status::success;
  }
  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::wait_pull(ValueType& elem, unique_lock<mutex>& lk)
  {
    //std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
    if (super::empty(lk))
    {
      //std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
      if (super::closed(lk)) return queue_op_status::closed;
    }
    //std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
    bool has_been_closed = super::wait_until_not_empty_or_closed(lk);
    //std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
    if (has_been_closed) return queue_op_status::closed;
    //std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
    pull(elem, lk);
    //std::cout << __FILE__ << "[" << __LINE__ << "]" << std::endl;
    return queue_op_status::success;
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::try_pull(ValueType& elem)
  {
    unique_lock<mutex> lk(super::mtx_);
    return try_pull(elem, lk);
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::wait_pull(ValueType& elem)
  {
    unique_lock<mutex> lk(super::mtx_);
    return wait_pull(elem, lk);
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::nonblocking_pull(ValueType& elem)
  {
    unique_lock<mutex> lk(super::mtx_, try_to_lock);
    if (!lk.owns_lock())
    {
      return queue_op_status::busy;
    }
    return try_pull(elem, lk);
  }

  template <class ValueType, class Container>
  void sync_queue<ValueType, Container>::pull(ValueType& elem)
  {
      unique_lock<mutex> lk(super::mtx_);
      super::wait_until_not_empty(lk);
      pull(elem, lk);
  }

  // enable if ValueType is nothrow movable
  template <class ValueType, class Container>
  ValueType sync_queue<ValueType, Container>::pull()
  {
      unique_lock<mutex> lk(super::mtx_);
      super::wait_until_not_empty(lk);
      return pull(lk);
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::try_push(const ValueType& elem, unique_lock<mutex>& lk)
  {
    if (super::closed(lk)) return queue_op_status::closed;
    push(elem, lk);
    return queue_op_status::success;
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::try_push(const ValueType& elem)
  {
    unique_lock<mutex> lk(super::mtx_);
    return try_push(elem, lk);
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::wait_push(const ValueType& elem, unique_lock<mutex>& lk)
  {
    if (super::closed(lk)) return queue_op_status::closed;
    push(elem, lk);
    return queue_op_status::success;
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::wait_push(const ValueType& elem)
  {
    unique_lock<mutex> lk(super::mtx_);
    return wait_push(elem, lk);
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::nonblocking_push(const ValueType& elem)
  {
    unique_lock<mutex> lk(super::mtx_, try_to_lock);
    if (!lk.owns_lock()) return queue_op_status::busy;
    return try_push(elem, lk);
  }

  template <class ValueType, class Container>
  void sync_queue<ValueType, Container>::push(const ValueType& elem)
  {
      unique_lock<mutex> lk(super::mtx_);
      super::throw_if_closed(lk);
      push(elem, lk);
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::try_push(BOOST_THREAD_RV_REF(ValueType) elem, unique_lock<mutex>& lk)
  {
    if (super::closed(lk)) return queue_op_status::closed;
    push(mars_boost::move(elem), lk);
    return queue_op_status::success;
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::try_push(BOOST_THREAD_RV_REF(ValueType) elem)
  {
    unique_lock<mutex> lk(super::mtx_);
    return try_push(mars_boost::move(elem), lk);
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::wait_push(BOOST_THREAD_RV_REF(ValueType) elem, unique_lock<mutex>& lk)
  {
    if (super::closed(lk)) return queue_op_status::closed;
    push(mars_boost::move(elem), lk);
    return queue_op_status::success;
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::wait_push(BOOST_THREAD_RV_REF(ValueType) elem)
  {
    unique_lock<mutex> lk(super::mtx_);
    return wait_push(mars_boost::move(elem), lk);
  }

  template <class ValueType, class Container>
  queue_op_status sync_queue<ValueType, Container>::nonblocking_push(BOOST_THREAD_RV_REF(ValueType) elem)
  {
    unique_lock<mutex> lk(super::mtx_, try_to_lock);
    if (!lk.owns_lock())
    {
      return queue_op_status::busy;
    }
    return try_push(mars_boost::move(elem), lk);
  }

  template <class ValueType, class Container>
  void sync_queue<ValueType, Container>::push(BOOST_THREAD_RV_REF(ValueType) elem)
  {
      unique_lock<mutex> lk(super::mtx_);
      super::throw_if_closed(lk);
      push(mars_boost::move(elem), lk);
  }

  template <class ValueType, class Container>
  sync_queue<ValueType, Container>& operator<<(sync_queue<ValueType, Container>& sbq, BOOST_THREAD_RV_REF(ValueType) elem)
  {
    sbq.push(mars_boost::move(elem));
    return sbq;
  }

  template <class ValueType, class Container>
  sync_queue<ValueType, Container>& operator<<(sync_queue<ValueType, Container>& sbq, ValueType const&elem)
  {
    sbq.push(elem);
    return sbq;
  }

  template <class ValueType, class Container>
  sync_queue<ValueType, Container>& operator>>(sync_queue<ValueType, Container>& sbq, ValueType &elem)
  {
    sbq.pull(elem);
    return sbq;
  }

}
using concurrent::sync_queue;

}

#include <third_party/boost/config/abi_suffix.hpp>

#endif