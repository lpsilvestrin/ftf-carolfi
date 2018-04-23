#!/bin/bash
echo "SDCs: "
grep -i $1 summary-carolfi.log | grep -i $2 | grep -i SDC |wc -l
echo "Hangs: "
grep -i $1 summary-carolfi.log | grep -i $2 | grep -i HANG |wc -l
echo "Masked: "
grep -i $1 summary-carolfi.log | grep -i $2 | grep -i Masked |wc -l
echo "Failed: "
grep -i $1 summary-carolfi.log | grep -i $2 | grep -i Failed |wc -l


