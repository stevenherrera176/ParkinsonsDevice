from sqlalchemy.sql.expression import table
from . import db

class SensorReadings(db.Model):
    '''model to keep track of sensor data'''
    __tablename__ = "sensor_data"
    _id = db.Column(db.Integer, primary_key=True, unique=True) # might not even need
    force_values = db.Column(db.String)
    gyro_values = db.Column(db.String)
    accel_values = db.Column(db.String)
    date_sent = db.Column(db.DateTime, unique=True)