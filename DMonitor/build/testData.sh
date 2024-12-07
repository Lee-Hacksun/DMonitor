#!/bin/bash

directory="$1"

if [ -z "$directory" ]; then
  echo "사용법: $0 <디렉토리>"
  exit 1
fi

if [ ! -d "$directory" ]; then
  mkdir -p "$directory"
fi

ClientList="$directory/ClientList.csv"
Progress="$directory/Progress.csv"
Sensor="$directory/Sensor.csv"

echo "clientID,Species,Red,Green,Blue,RegionCode" > "$ClientList"

species=("CherryBlossom" "Maple")
region_codes=(00 11 21 22 23 24 25 26 29 31 32 33 34 35 36 37 38 39)

for i in $(seq 1 200); do
  clientID=$i
  random_species=${species[$RANDOM % ${#species[@]}]}
  red=$((RANDOM % 256))
  green=$((RANDOM % 256))
  blue=$((RANDOM % 256))
  random_region_code=${region_codes[$RANDOM % ${#region_codes[@]}]}

  echo "$clientID,$random_species,$red,$green,$blue,$random_region_code" >> "$ClientList"
done

echo "ClientID,Progress" > "$Progress"

for i in $(seq 1 200); do
  clientID=$i
  progress=$((RANDOM % 101))

  echo "$clientID,$progress" >> "$Progress"
done

echo "ClientID,flame,gas,humidity,light,temp,Red,Green,Blue,Progress" > "$Sensor"

for i in $(seq 1 5000); do
  clientID=$(( (i - 1) % 200 + 1 ))
  gas=$((RANDOM % 1000))  
  flame=$((RANDOM % 2))
  humidity=$((RANDOM % 80))  
  light=$((RANDOM % 1000))  
  temp=$((RANDOM % 40))  
  red=$((RANDOM % 256))  
  green=$((RANDOM % 256))  
  blue=$((RANDOM % 256))  
  progress=$((RANDOM % 101))  

  echo "$clientID,$flame,$gas,$humidity,$light,$temp,$red,$green,$blue,$progress" >> "$Sensor"
done
