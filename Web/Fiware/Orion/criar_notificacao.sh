#!/bin/bash
echo "Criando Notificacao no Cygnus..."
(curl 35.184.233.148:1026/v1/subscribeContext -s -S --header 'Content-Type: application/json' \
    --header 'Accept: application/json' --header 'Fiware-Service: tcc' --header 'Fiware-ServicePath: /' -d @- | python -mjson.tool) <<EOF
{
    "entities": [
        {
            "type": "Colmeia",
            "isPattern": "false",
            "id": "$1"
        }
    ],
    "attributes": [
        "temperatura",
		"umidade",
		"data",
		"hora",
		"som",
		"dioxido",
		"tensaocolmeia",
		"tensaorepetidor"
    ],
    "reference": "http://10.128.0.5:5050/notify",
    "duration": "P100Y",
    "notifyConditions": [
        {
            "type": "ONCHANGE",
            "condValues": [
            ]
        }
    ],
    "throttling": "PT3S"
}
EOF

