#!/bin/bash
#file="weather_data.txt"
gcc -o weart weart.c cJSON.c -lcurl
./weart
file="weather_data.txt"
# Read data from the file
data=$(cat "$file")

# Display data to the user
echo "Environmental Report:"
echo "$data"

# Extract temperature from the data (adjust the field name based on your data structure)
temperature=$(echo "$data" | grep -oP 'Temperature: \K[0-9.]+')

# Weather thresholds
threshold_temperature_high=30
threshold_temperature_low=12

# Elastic Email credentials
elastic_email_username="wagob90084@visignal.com"
elastic_email_password="29D627F6EC2F6E222D408489EC8F7DE13998"

# Email details
from_email="wagob90084@visignal.com"
to_email="s.m.aijaz25@gmail.com"

# Check if the temperature is a valid number
if [[ ! $temperature =~ ^[0-9.]+$ ]]; then
    echo "Error: Invalid temperature value in the data."
    exit 1
fi

# Check if the temperature is higher than the threshold
if (( $(echo "$temperature > $threshold_temperature_high" | bc -l) )); then
    subject="High Temperature Alert"
    body="The current temperature ($temperature) is higher than the threshold ($threshold_temperature_high)."
    sendemail -f "$from_email" -t "$to_email" -u "$subject" -m "$body" \
        -s smtp.elasticemail.com:2525 -o tls=yes -xu "$elastic_email_username" -xp "$elastic_email_password"
    echo "Email sent."
elif (( $(echo "$temperature < $threshold_temperature_low" | bc -l) )); then
    subject="Low Temperature Alert"
    body="The current temperature ($temperature) is lower than the threshold ($threshold_temperature_low)."
    sendemail -f "$from_email" -t "$to_email" -u "$subject" -m "$body" \
        -s smtp.elasticemail.com:2525 -o tls=yes -xu "$elastic_email_username" -xp "$elastic_email_password"
    echo "Email sent."
else
    echo "Temperature is within the normal range."
fi

