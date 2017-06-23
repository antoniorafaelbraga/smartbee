#!/bin/bash
#Script tem que receber 5 parametros, senão não executará
if [ $# -lt 9 ]; then
   echo "./criar_entidade [id] [tipo] [temperatura] [umidade] [som] [data] [hora] [co2] [tensaocolmeia] [tensaorepetidor]"
   exit 1
fi

(curl 35.184.233.148:1026/v1/updateContext -s -S --header 'Content-Type: application/json' \
    --header 'Accept: application/json' --header 'Fiware-Service: tcc' --header 'Fiware-ServicePath: /' -d @- | python -mjson.tool) <<EOF
{
    "contextElements": [
        {
            "type": "$2",
            "isPattern": "false",
            "id": "$1",
            "attributes": [
                {
                    "name": "temperatura",
                    "type": "float",
                    "value": "$3"
                },
				{
                    "name": "umidade",
                    "type": "float",
                    "value": "$4"
                },
				{
                    "name": "som",
                    "type": "float",
                    "value": "$5"
                },
				{
                    "name": "data",
                    "type": "string",
                    "value": "$6"
                },
				{
                    "name": "hora",
                    "type": "string",
                    "value": "$7"
                },
				{
                    "name": "dioxido",
                    "type": "float",
                    "value": "$8"
                },
				{
                    "name": "tensaocolmeia",
                    "type": "float",
                    "value": "$9"
                },
				{
                    "name": "tensaorepetidor",
                    "type": "float",
                    "value": "$10"
                }
            ]
        }
    ],
    "updateAction": "UPDATE"
} 
EOF

