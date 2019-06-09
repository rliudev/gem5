#
# Copyright (c) 2019 Rach Liu
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Author: Rach Liu

from m5.SimObject import *
from m5.params import *
from m5.proxy import *

from m5.objects.ClockedObject import ClockedObject


class PerceptronUnit(ClockedObject):
    type = 'PerceptronUnit'
    cxx_class = "PerceptronUnit"
    cxx_header = "mem/cache/prefetch/perceptron_unit.hh"

    reject_all = Param.Bool(False, "Reject all prefetch addresses.")
    accept_all = Param.Bool(False, "Accept all prefetch addresses.")
    exponential_size = Param.Int(32, "Table size for Perceptrons")
    perceptron_size = Param.Int(20, "Feature dimensions (size of each erceptron)")
    pf_timeout = Param.Int(19,
        "Number of cache accesses (hits or misses) before a prefetch is considered a timeout.")

