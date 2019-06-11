/*
 * 2019 Rach Liu -- Refactored out of prefetch/base.hh
 *
 * Copyright (c) 2013-2014 ARM Limited
 * All rights reserved.
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2005 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Ron Dreslinski
 *          Mitch Hayenga
 */

#ifndef __MEM_CACHE_PREFETCH_PREFETCHINFO_HH__
#define __MEM_CACHE_PREFETCH_PREFETCHINFO_HH__

#include <cstdint>

#include "arch/isa_traits.hh"
#include "base/statistics.hh"
#include "base/types.hh"
#include "mem/packet.hh"
#include "mem/request.hh"
#include "sim/byteswap.hh"
#include "sim/clocked_object.hh"
#include "sim/probe/probe.hh"


/**
 * Class containing the information needed by the prefetch to train and
 * generate new prefetch requests.
 */
class PrefetchInfo {
  /** The address used to train and generate prefetches */
  Addr address;
  /** The program counter that generated this address. */
  Addr pc;
  /** The requestor ID that generated this address. */
  MasterID masterId;
  /** Validity bit for the PC of this address. */
  bool validPC;
  /** Whether this address targets the secure memory space. */
  bool secure;
  /** Size in bytes of the request triggering this event */
  unsigned int size;
  /** Whether this event comes from a write request */
  bool write;
  /** Physical address, needed because address can be virtual */
  Addr paddress;
  /** Whether this event comes from a cache miss */
  bool cacheMiss;
  /** Pointer to the associated request data */
  uint8_t *data;

  public:
  /**
   * Obtains the address value of this Prefetcher address.
   * @return the addres value.
   */
  Addr getAddr() const
  {
    return address;
  }

  /**
   * Returns true if the address targets the secure memory space.
   * @return true if the address targets the secure memory space.
   */
  bool isSecure() const
  {
    return secure;
  }

  /**
   * Returns the program counter that generated this request.
   * @return the pc value
   */
  Addr getPC() const
  {
    assert(hasPC());
    return pc;
  }

  /**
   * Returns true if the associated program counter is valid
   * @return true if the program counter has a valid value
   */
  bool hasPC() const
  {
    return validPC;
  }

  /**
   * Gets the requestor ID that generated this address
   * @return the requestor ID that generated this address
   */
  MasterID getMasterId() const
  {
    return masterId;
  }

  /**
   * Gets the size of the request triggering this event
   * @return the size in bytes of the request triggering this event
   */
  unsigned int getSize() const
  {
    return size;
  }

  /**
   * Checks if the request that caused this prefetch event was a write
   * request
   * @return true if the request causing this event is a write request
   */
  bool isWrite() const
  {
    return write;
  }

  /**
   * Gets the physical address of the request
   * @return physical address of the request
   */
  Addr getPaddr() const
  {
    return paddress;
  }

  /**
   * Check if this event comes from a cache miss
   * @result true if this event comes from a cache miss
   */
  bool isCacheMiss() const
  {
    return cacheMiss;
  }

  /**
   * Gets the associated data of the request triggering the event
   * @param Byte ordering of the stored data
   * @return the data
   */
  template <typename T>
    inline T
    get(ByteOrder endian) const
    {
      if (data == nullptr) {
        panic("PrefetchInfo::get called with a request with no data.");
      }
      switch (endian) {
        case BigEndianByteOrder:
          return betoh(*(T*)data);

        case LittleEndianByteOrder:
          return letoh(*(T*)data);

        default:
          panic("Illegal byte order in PrefetchInfo::get()\n");
      };
    }

  /**
   * Check for equality
   * @param pfi PrefetchInfo to compare against
   * @return True if this object and the provided one are equal
   */
  bool sameAddr(PrefetchInfo const &pfi) const
  {
    return this->getAddr() == pfi.getAddr() &&
      this->isSecure() == pfi.isSecure();
  }

  /**
   * Constructs a PrefetchInfo using a PacketPtr.
   * @param pkt PacketPtr used to generate the PrefetchInfo
   * @param addr the address value of the new object, this address is
   *        used to train the prefetcher
   * @param miss whether this event comes from a cache miss
   */
  PrefetchInfo(PacketPtr pkt, Addr addr, bool miss);

  /**
   * Constructs a PrefetchInfo using a new address value and
   * another PrefetchInfo as a reference.
   * @param pfi PrefetchInfo used to generate this new object
   * @param addr the address value of the new object
   */
  PrefetchInfo(PrefetchInfo const &pfi, Addr addr);

  ~PrefetchInfo()
  {
    delete data;
  }
};


#endif //__MEM_CACHE_PREFETCH_PREFETCH_INFO_HH__

