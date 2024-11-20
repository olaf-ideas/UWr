import random
import time

def generate_log_file(file_name, num_lines):
    methods = ["GET", "POST", "PUT", "DELETE"]
    urls = [
        "/TheApplication/WebResource.axd",
        "/api/data",
        "/index.html",
        "/images/logo.png",
        "/css/styles.css"
    ]
    status_codes = [200, 301, 404, 500]
    
    
    ips = []
    for i in range(100):
        for j in range(random.randint(1, 5)):
            ips.append(".".join(str(random.randint(0, 255)) for _ in range(4)))
        
        
    with open(file_name, 'w') as file:
        for _ in range(num_lines):
            timestamp = time.strftime("%H:%M:%S", time.gmtime(random.randint(0, 86400)))
            
            ip_address = ips[random.randint(0, len(ips) - 1)] #".".join(str(random.randint(0, 255)) for _ in range(4))
            
            method = random.choice(methods)
            url = random.choice(urls)
            status = random.choice(status_codes)
            
            log_line = f"{timestamp} {ip_address} {method} {url} {status}\n"
            
            file.write(log_line)

generate_log_file("server_logs.txt", 1000)