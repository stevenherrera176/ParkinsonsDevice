from flask import Blueprint, request
from .models import SensorReadings
from datetime import datetime
from . import db
import requests

views = Blueprint('views', __name__)

@views.route("/", methods=["GET"])
def root():
    return "Welcome to Data Collector !!"

@views.route("/parkinsons", methods=["POST"]) #define route and method on web server
def post_data():

    force_values = request.form.get("force_values") #define keys for recieving POST requests
    gyro_values = request.form.get("gyro_values")
    accel_values = request.form.get("accel_values")

    #store to the database
    data = SensorReadings(
        force_values=force_values, #sort data before sending to the database
        gyro_values=gyro_values,
        accel_values=accel_values,
        date_sent=datetime.now()
        )
    db.session.add(data)
    db.session.commit() #send data to SQL database

    return f"gyro sensor has provided gyro value {gyro_values} and accel value {accel_values}, Force sensors have provided force values {force_values}"