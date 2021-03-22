#!/bin/bash

echo "<h1>bienvenue poste ${REMOTE_ADDR}</h1>"

# determiner si c'est le GET
if [ "$REQUEST_METHOD" == "GET" ]
  then
    # si oui, donner le valeur a CHAINE
    CHAINE=$QUERY_STRING
  else
    read CHAINE
fi 

# convert toutes les & en blanc 
LISTE=${CHAINE//&/ }

for param in $LISTE
do
  # Supprimer la chaine correspondante ‘=’ la plus court de la fin fr la chaine 
  champ=${param%=*}
  valeur=${param#*=}
  # donner les valeur a des variables
  case $champ in
    temperature)
      T=$valeur ;;
    humidite)
      H=$valeur ;;
    pression)
      P=$valeur ;;
  esac
 
  # output les variables a un fichier
  echo "les valeurs recu : " >> ./data.csv
  echo "temperature = $T" >> ./data.csv
  echo "humidite = $H" >> ./data.csv
  echo "pression = $P" >> ./data.csv
  echo "" >> ./data.csv
