Weather Alert System
This project fetches real-time weather data for a specified city, writes the data to a text file, and sends an email alert if the temperature exceeds certain thresholds. The project is built in C and utilizes the cURL and cJSON libraries to interact with the OpenWeatherMap API. Additionally, a Bash script is used to handle email notifications via the Elastic Email service.

Features
Fetches real-time weather data for a specified city using the OpenWeatherMap API.
Parses the weather data to extract temperature and "feels like" values.
Converts temperature from Kelvin to Celsius.
Writes the weather data to a text file.
Reads the temperature data and compares it with predefined thresholds.
Sends an email alert if the temperature is above or below the specified thresholds.
Prerequisites
GCC: For compiling the C program.
cURL: For making HTTP requests.
cJSON: For parsing JSON data.
Elastic Email Account: For sending email alerts.
sendemail: A lightweight command line SMTP email client.
Configuration
API Key and City Name:
Modify the following constants in the weart.c file:

c
Copy code
#define API_KEY "your_openweather_api_key"
#define CITY_NAME "your_city_name"
Email Settings:
Configure your email credentials in the Bash script (weather_alert.sh):

bash
Copy code
elastic_email_username="your_elastic_email_username"
elastic_email_password="your_elastic_email_password"
from_email="your_email"
to_email="recipient_email"
Temperature Thresholds:
Set your desired high and low temperature thresholds in the Bash script:

bash
Copy code
threshold_temperature_high=30
threshold_temperature_low=12
Usage
Fetch and Store Weather Data:
The C program fetches weather data and writes it to weather_data.txt.

Trigger Alerts:
The Bash script reads the temperature from the file and sends an email alert if it crosses the predefined thresholds.
