# Author: Rach Liu

from __future__ import print_function
from __future__ import absolute_import

import m5
from m5.objects import *
from .Caches import *
from . import HWPConfig


def addMLUOptions(parser):
    # Command line options
    parser.add_option("--mlu_pf_type", action="store",
                      default=None, type="string",
                      help="If specified, enables the given MLU type for prefetch")


def config_mlu(options, system):
  if options.l2cache:
    if system.l2.prefetcher:
      if options.mlu_pf_type == "perceptron":
        try:
          system.l2.prefetcher.perceptron_unit = PerceptronUnit()
          print("MLU-Prefetch Perceptron Enabled.")
        except NameError:
          print("Warning: no PerceptronUnit defined. Ignoring PerceptronUnit insertion")
      else:
        print("Warning: no l2 prefetcher specified. Ignoring L2 Prefetch MLU config")
    else:
      print("Warning: no l2 specified. Ignoring L2 MLU config")


