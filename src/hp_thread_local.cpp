// Copyright (c) 2020-2020 Alexander Gaev
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cds/gc/hp.h>
#include <map>
#include <unordered_map>

namespace cds {
  namespace gc {
    namespace hp {
      thread_local thread_data *tls_ = nullptr;
      thread_local std::pair<thread_data *, thread_data *> *tls2_ = new std::pair<thread_data *, thread_data *>(nullptr,
                                                                                                                nullptr);
      static std::unordered_map<cds::OS::posix::ThreadId, thread_data *> heap_tls_ = std::unordered_map<cds::OS::posix::ThreadId, thread_data *>(100);

      /*static*/ CDS_EXPORT_API thread_data *&DefaultSMRManager::getTLS() {
        return tls_;
      }

      /*static*/ CDS_EXPORT_API thread_data *&StrangeSMRManager::getTLS() {
        if (cds::OS::get_current_thread_id() % 2 == 0) { // % 2 with ThreadId structure?
          return tls2_->second;
        } else {
          return tls2_->first;
        }
      }

      /*static*/ CDS_EXPORT_API thread_data *&HeapSMRManager::getTLS() {
        cds::OS::posix::ThreadId thread_id = cds::OS::get_current_thread_id();
        return heap_tls_[thread_id]; // unsafe if many threads are trying to use getTLS
      }
    }
  }
}