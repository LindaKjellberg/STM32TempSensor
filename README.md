# STM32TempSensor

Under vintern sjunker temperaturen och luftfuktigheten utomhus såväl som inomhus. Det förändrade inomhusklimatet skapar inte bara obehag för mig, men det påfrestar mina växter avsevärt mycket. Vissa växter verkar ta skada trotts regelbunden bevattning och så mycket dagsljus som dagen tillåter, därför tittar det här projektet på hur temperaturen och luftfuktigheten kan påverka inomhusväxters hälsa.

Från en fysisk Device som består av en DHT22 sensor kopplad till en ESP32 PCB skickas data över WIFI till AWS IoT Core, sen lagras mätvärden i en TimeStream databas. Genom att skapa denna "Device to Cloud"-lösning kan jag samla in temperatur och luftfuktighet inomhus och lagra data i molnet, där jag vidare kan hämta data och skapa grafer i visualiseringsverktyget Grafana. För att skydda känsliga lösenord och certifikat separerades dessa från huvudkoden i en egen fil som sen döljs med hjälp av gitignore.

![Alt text](image-4.png)


### _Hårdvaru-komponenter:_

- DHT22 (AM2302)
- ESP32 micrococtroller med WIFI
- 10 kOhm resistor
- Strömkälla (USB-kabel till min laptop)
- Jumperkablar

![Alt text](image-3.png)


### _Kopplingsschema:_

- (+) 3.3V pin till DHT22 pin 1
- (-) GND till DHT22 pin 4
- GPIO D4 med 10kOhm resistor till DHT22 pin 2
- DHT22 pin 3 är inte kopplad

![Alt text](image-2.png)


### _Mjukvarukomponenter:_
- Arduino IDE
- AWS IoT Core
- (DynamoDB)
- TimeStreamDB
- Visualisering med Grafana 


### _Databasen:_

I TimeStream korttids lagras temperatur- och luftfuktighets värden tillsammans med device_id och en timestamp som visar tiden mätningen gjordes.

![Alt text](image-7.png)


### _Grafana:_

Jag använde Grafana för att visualisera den insamlade datan i grafer:

![Alt text](image-6.png)


### Motgångar och hinder

Tanken var att kunna låta min Device mäta och skicka data till AWS utan att vara inkopplad i datorn, men jag fick inte strömförsörjningen att fungera med en power bank som var den fristående strömkälla jag hade att använda. Jag ville att lösningen skulle vara "portabel", och i efterhand ångrar jag att jag inte testade att koppla in USB-kabeln i vägguttaget för att på så sätt kunna testa lösningen via nätverksporten.

I AWS stötte jag på hinder med att använda DynamoDB för lagring av data och sen hämta data till visualiseringsverktyget Grafana. Eftersom detta inte fungerade så bestämde jag mig för att i stället använda TimeStream för att lagra data och koppla på Grafana externt för visualiseringen.

Eftersom jag valde att arbeta med TimeStream i stället för DynamoDB så bytte jag region från Stockholm till Irland i AWS. Jag valde att samla alla mina tjänster i AWS region Irland för att underlätta översikten av projektet, men detta relaterade i att tiden visar en timma fel då Stockholm ligger en timma före Irland.