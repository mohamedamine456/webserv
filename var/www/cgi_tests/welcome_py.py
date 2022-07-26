#import cgi module:
import cgi
 
#output http header:
# print ('Content-type: text/html')
print ('')
# note the empty print above is required!
 
print ('<html><head><title>Python Test</title></head>')
print ('<body>')
 
theRequest = cgi.FieldStorage()
name = theRequest.getfirst("name", "undefined")
print ('<h3>First Test OF Python CGI</h3>')
print("Name of the user is " + name)
sport = theRequest.getfirst("sport", "undefined")
print(" plays " + sport + " as Sport")
year = theRequest.getfirst("year", "undefined")
print("since " + year + "!")

print ('</body></html>')