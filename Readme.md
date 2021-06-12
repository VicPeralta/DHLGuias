# DHL Guias

This an interface to DHL's web services to generate a shipment request and its shipping label.

## Development
This program is written in C++ using Visual Studio 2019

It performs this operation:
* Open an XML file with the information of the shipment request, it must contain all the information required by the Server:
    * Billing
    * Consignee
    * Shipment Details
    * Shipper
    * Required label format (PDF, ZPL)


* Connect to DHL Web server and POST the shipment request
* Receive and parse the response
* If the response is successful decode the label format and write it to a file
* Keep a record of request-airway bill number


### Prerequisites
* [Visual Studio 2019](https://visualstudio.microsoft.com/es/) Community Edition
* [POCO](https://pocoproject.org/)  library if additional functionality is required

## Usage
```
dhlguias.exe [server] [xml file] [output directory]
Example:
dhlguias.exe http://xmlpitest-ea.dhl.com/XMLShippingServlet c:\data\requestPdf.xml c:\data\airwaybills
```

## Sample captures
![alt text](captures/airwaybill.png?raw=true "Airwaybill")
![alt text](captures/call.png?raw=true "available")

## Authors
* **Victor Peralta** -(victor.peralta.gomez@gmail.com)
