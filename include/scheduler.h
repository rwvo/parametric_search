// file scheduler.h

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <cstddef>
#include <list>
#include <deque>
#include <memory>
#include <functional>

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits >
  class Scheduler
  //--------------------------------------------------
  {
  public: // type definitions
    typedef typename Traits::number_t                  number_t;
    typedef typename Traits::process_base_t            process_base_t;
    typedef typename Traits::comparison_base_t         comparison_base_t;
    typedef typename Traits::root_t                    root_t;
    typedef typename Traits::solver_base_t             solver_base_t;

    typedef 
    typename std::list< process_base_t* >::iterator    process_iter_t;
    typedef 
    typename std::list< comparison_base_t* >::iterator comparison_iter_t;
    typedef 
    typename std::deque< root_t* >::iterator           root_iter_t;

  public: // member functions
    Scheduler();
    ~Scheduler();
    static Scheduler< Traits >* instance();

    bool is_running() const;
    const number_t& lower_bound() const;
    const number_t& upper_bound() const;

    std::size_t no_computations() const;
    std::size_t no_processes() const;
    std::size_t no_comparisons() const;
    std::size_t no_comparisons_total() const;
    std::size_t no_roots() const;

    void inc_no_comparisons_total();

    void increase_lower_bound_to( const number_t& bound );
    void decrease_upper_bound_to( const number_t& bound );

    void run();
    void reset();

    process_iter_t    schedule( process_base_t* process );
    comparison_iter_t schedule( comparison_base_t* comparison );
    void              schedule( root_t* root );

    process_iter_t suspend( process_iter_t process_iter );
    process_iter_t reactivate( process_iter_t process_iter );
    void terminate( process_iter_t process_iter );

    void reactivate( comparison_iter_t comparison_iter );
    void rootless( comparison_iter_t comparison_iter );

    void register_solver( solver_base_t* solver );

  private: // member functions
    // Since this is an (implementation class for a) singleton class,
    // the copy constructor and assignment operator are private and
    // not implemented


    Scheduler( const Scheduler& );            // not implemented
    Scheduler& operator=( const Scheduler& ); // not implemented

    void cleanup();
    bool have_deducable_roots() const;
    void resume_processes_and_deduce_roots();
    void resolve_root( const number_t& value );
    void find_and_resolve_median_root();

  private: // data members
    std::list< process_base_t* >    active_processes_;     // owned
    std::list< process_base_t* >    suspended_processes_;  // owned
    std::list< comparison_base_t* > comparisons_;          // owned
    std::list< comparison_base_t* > rootless_comparisons_; // owned
    std::deque< root_t* >           roots_;                // owned
    solver_base_t*                  solver_;

    bool      running_;
    number_t  lower_bound_;
    number_t  upper_bound_;

    std::size_t no_computations_;
    std::size_t no_processes_;
    std::size_t no_comparisons_;
    std::size_t no_comparisons_total_;
    std::size_t no_roots_;

  private: // static data members
    static std::auto_ptr< Scheduler< Traits > > instance_;

  private: // nested function object classes
    struct Can_be_deduced : public std::unary_function< root_t*, bool > {
      bool operator()( root_t* root ) const;
    };

    struct Root_comp {
      bool operator()( root_t* lhs, root_t* rhs ) const;
    };

  };

} // namespace ns_parametric_search


#endif // SCHEDULER_H
