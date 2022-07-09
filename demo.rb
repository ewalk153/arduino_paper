require 'sinatra'
set :bind, '0.0.0.0'
set :port, 80

get '/api/states/sensor.office_temperature' do
  puts request.inspect
  <<~ENDTEXT 
   {"entity_id": "sensor.office_temperature", "state": "25.7", "attributes": {"state_class": "measurement", "unit_of_measurement": "\u00b0C", "device_class": "temperature", "friendly_name": "Office Temperature"}, "last_changed": "2022-06-22T15:45:16.780952+00:00", "last_updated": "2022-06-22T15:45:16.780952+00:00", "context": {"id": "01G661DPFCWWM8D3KARDCXW9F8", "parent_id": null, "user_id": null}}
  ENDTEXT
end
