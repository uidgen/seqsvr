#!/usr/bin/env bash

echo "run store ..."
nohup ./store -f=../etc/store.yaml >> ../logs/store.log  2>&1 &
sleep 1

echo "run mediate ..."
nohup ./mediate -f=../etc/mediate.yaml >> ../logs/mediate.log  2>&1 &
sleep 1

echo "run alloc ..."
nohup ./alloc -f=../etc/alloc.yaml >> ../logs/alloc.log  2>&1 &
sleep 1
