import http.client
import requests
import urllib
import xml.etree.ElementTree as ET
import hashlib
import base64
import re
import time
import datetime
import argparse

# Python3

#
# Router password SHA256 hexdigest
# Use the 'admin' password.
#
# How to get password hexdigest:
#
# Mac/Linux:   echo -n 'your password here' | shasum -a 256
#
# Example value (password = password):
#
# passwordSHA256 = '5e884898da28047151d0e56f8dc6292773603d0d6aabbdd62a11ef721d1542d8'

passwordSHA256 = '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'

#
# Version
#
version = '1.1.0'

class Hwcli:

    def __init__(self, baseUrl, username = 'admin', password = ''):
        self.baseUrl = baseUrl
        self.port = 80
        self.username = username
        self.password = password
        self.token = None
        self.sessionInfo = None
        self.session = requests.Session()
        self.cookie = None
        self.rvt0 = None
        self.rvt1 = None
        self.rvt2 = None
        self.debug = False

    def apiUrl(self, api = ''):
        return 'http://{}{}{}'.format(self.baseUrl, '' if self.port == 80 else ':{}'.format(self.port), api)

    def apiPost(self, api = '', data = None):
        self._getSession()
        r = self.session.post(self.apiUrl(api), data = data)
        if self.debug:
            print(r.url, '->', r.status_code , '\n')
        arr = [
            '__RequestVerificationToken',
            '__RequestVerificationTokenone',
            '__RequestVerificationTokentwo'
            ]
        for k in arr:
            try:
                self.session.headers.update({ k: r.headers[k] })
            except KeyError as e:
                pass
        return r

    def apiGet(self, api = ''):
        self._getSession()
        r = self.session.get(self.apiUrl(api))
        if self.debug:
            print(r.url, '->', r.status_code)
        return r

    def _getSession(self):

        if self.token != None and self.sessionInfo != None:
            return

        request = self.session.get('http://' + self.baseUrl + '/api/webserver/SesTokInfo')

        data = request.text
        tree = ET.fromstring(data)
        if len(tree.findall('TokInfo')) > 0:
            self.token = tree.findall('TokInfo')[0].text
        else:
            self.token = ''
            if self.debug:
                print('Failed to get session token')
        if len(tree.findall('SesInfo')) > 0:
            self.sessionInfo = tree.findall('SesInfo')[0].text
        else:
            self.sessionInfo = ''
            if self.debug:
                print('Failed to get session info')

    def login(self):
        print("\ninit login ...\n")
        self._getSession()

        self.session.headers.update({ 'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8',
                                      'Cookie': self.sessionInfo,
                                      '__RequestVerificationToken': self.token
                                      })
        print("headers ::::: " , self.session.headers , "\n")
        print("token :" + self.token + "\n")
        print("sessionInfo :" + self.sessionInfo + "\n")
        print("username :" + self.username + "\n")
        print("authToken : " + self._authToken() + "\n")

        xml = '<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<request>\n <Username>{}</Username>\n <Password>{}</Password>\n <password_type>4</password_type>\n</request>\n'.format(self.username, self._authToken())
        p = self.apiPost('/api/user/login', xml)
        xml = p.text

        tree = ET.fromstring(xml)

        if tree.text != 'OK':
            raise Exception('login: Login failed.')

        # Cookie
        try:
            cookie = p.headers['Set-Cookie']
            self.cookie = cookie
            self.session.headers.update({ 'Cookie': cookie })

            self.rvt0 = p.headers['__RequestVerificationToken']
            self.rvt1 = p.headers['__RequestVerificationTokenone']
            self.rvt2 = p.headers['__RequestVerificationTokentwo']

            self.session.headers.update({ '__RequestVerificationToken': self.rvt0 })
            self.session.headers.update({ '__RequestVerificationTokenone': self.rvt1 })
            self.session.headers.update({ '__RequestVerificationTokentwo': self.rvt2 })
        except KeyError as e:
            raise Exception('login: Missing key {}.'.format(str(e)))

    def _authToken(self):

        if self.token == None or self.token == '': raise ValueError('error: No valid authentication token available.')

        def encrypt(text):
            m = hashlib.sha256()
            m.update(str.encode(text))
            return base64.b64encode(str.encode(m.hexdigest())).decode()
        #print("content : " , self.username + base64.b64encode(str.encode(self.password)).decode() + self.token)
        return encrypt(self.username + base64.b64encode(str.encode(self.password)).decode() + self.token)

    def dumpXMLFields(self, matchingFields, content):
        root = ET.fromstring(content)
        if matchingFields == None or len(matchingFields) < 1:
            ET.dump(root)
        else:
            for needle in matchingFields:
                val = root.findall(needle)
                if type(val) is list:
                    if len(val) == 0:
                        print('No matches for \'{}\''.format(needle))
                    else:
                        def _prettyprint(item, lvl):
                            pad = ' '
                            if type(item) is not list:
                                children = list(item)
                                if len(children) > 0: _prettyprint(children, lvl + 2)
                                else: print('{}<{}>{}</{}>'.format(pad * lvl, item.tag, item.text if item.text is not None else '', item.tag))
                            else:
                                for i in item:
                                    children = list(i)
                                    if children != None: 
                                        print('{}<{}>'.format(pad * lvl, i.tag), end = '')
                                        if len(children) > 0:
                                            print()
                                            _prettyprint(children, lvl + 2)
                                            print('{}</{}>'.format(pad * lvl, i.tag))
                                        else:
                                            print('{}</{}>'.format(i.text if i.text is not None else '', i.tag))



                        lvl = 0
                        for index,i in enumerate(val):
                            _prettyprint(i, lvl)
                else:
                    print('***', val)

    def apiDeviceConfigXml(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/config/deviceinformation/config.xml').content)
    def apiGlobalConfigXml(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/config/global/config.xml').content)
    def apiSmsConfig(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/config/sms/config.xml').content)
    def apiMonitoringTrafficStatistics(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/monitoring/traffic-statistics').content)
    def apiWlanFeature(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/wlan/wifi-feature-switch').content)
    def apiWlanHostList(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/wlan/host-list').content)
    def apiMonitoringStatus(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/monitoring/status').content)
    def apiSimStatus(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/pin/status').content)
    def apiIsLoggedIn(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/user/state-login').content)
    def apiWlanBasicSettings(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/wlan/basic-settings').content)
    def apiWlanHandoverSettings(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/wlan/handover-setting').content)
    def apiGlobalModuleSwitch(self, fields):
        self.dumpXMLFields(fields, self.apiGet('/api/global/module-switch').content)
        
    def apiSmsInbox(self, fields = None):
        xml = '<?xml version="1.0" encoding="UTF-8"?><request><PageIndex>1</PageIndex><ReadCount>10</ReadCount><BoxType>1</BoxType><SortType>0</SortType><Ascending>0</Ascending><UnreadPreferred>1</UnreadPreferred></request>'
        self.dumpXMLFields(fields, self.apiPost('/api/sms/sms-list', xml).content)

    def apiSendSms(self, recipients = None, message = None):
        if recipients == None or message == None: return
        assert(type(recipients) is list)
        assert(type(message) is str)
        now = datetime.datetime.now()
        future = now + datetime.timedelta(minutes = 1)
        plist = ''
        for phone in recipients:
            if type(phone) is str: plist += '<Phone>{}</Phone>'.format(phone)
            else: print('Invalid recipient \'{}\' skipped.'.format(str(phone)))

        xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><request><Index>-1</Index><Phones>" + plist + "</Phones><Sca></Sca><Content>" + message + "</Content><Length>" + str(len(message)) + "</Length><Reserved>1</Reserved><Date>" + future.strftime('%Y-%m%d %H:%M:%S') + "</Date></request>"
        r = self.apiPost('/api/sms/send-sms', str(xml))

    def apiReboot(self, fields = None):
        xml = '<?xml version="1.0" encoding="UTF-8"?>\n<request>\n <Control>1</Control>\n</request>'
        self.dumpXMLFields(fields, self.apiPost('/api/device/control', data = xml).content)

    def apiDataSwitch(self,on=True):
        if on:
            xml = '<?xml version="1.0" encoding="UTF-8"?>\n<request>\n <dataswitch>1</dataswitch>\n</request>'
        else:
            xml = '<?xml version="1.0" encoding="UTF-8"?>\n<request>\n <dataswitch>0</dataswitch>\n</request>'
        self.dumpXMLFields(None, self.apiPost('/api/dialup/mobile-dataswitch', data = xml).content)

    def apiShowMacFilter(self, fields = None):
        self.dumpXMLFields(fields, self.apiGet('/api/security/mac-filter').content)

    mac_filter_types = {
        'disabled': 1,
        'allow':    2,
        'deny':     3
    }

    def apiSetMacFilter(self, mac_list, mode):
        xml = '<?xml version="1.0" encoding="UTF-8"?><request><policy>' + str(mode) + '</policy><macfilters>\n'
        for mac_address in mac_list:
            xml += '<macfilter><value>' + mac_address + '</value><status>1</status></macfilter>\n'
        xml += '</macfilters></request>\n'
        if self.debug:
            print('xml: ' + xml)
        r = self.apiPost('/api/security/mac-filter', str(xml))

def parseArgumentsAndRun():
    global passwordSHA256
    global version

    parser = argparse.ArgumentParser()

    subParsers = parser.add_subparsers(title='commands', dest = 'commands')

    # Configure 'main parser'                                            
    parser.add_argument('-v', '--version', action='version', version = '%(prog)s {}'.format(version), help = 'Display version number')
    parser.add_argument('-d', '--debug', action = 'store_true', default = False, help = 'Show debug information.')
    parser.add_argument('-t', metavar = 'address', type=str, default = '192.168.8.1', help = 'Target hostname or IP-address')
    parser.add_argument('-s', metavar = 'sha256', type=str, default = None, help = 'Router\'s password as SHA256 hexdigest.')

    # Create 'sub parsers'
    show_parser   = subParsers.add_parser('show', help = 'Show router information.')
    data_parser   = subParsers.add_parser('data', help = 'Enable or disable mobile data.')
    reboot_parser = subParsers.add_parser('reboot', help = 'Reboot router.')
    send_parser   = subParsers.add_parser('sendsms', help = 'Send SMS message.')
    filter_parser = subParsers.add_parser('mac-filter', help = 'Set MAC filtering.')

    # Config 'show'
    showSubParsers = show_parser.add_subparsers(title='item', dest='item', help = 'Available items')

    showarr = {
        'sim':           ('SIM status.', 'apiSimStatus'),
        'monitoring':    ('Monitoring status.', 'apiMonitoringStatus'),
        'traffic':       ('Traffic statistics.', 'apiMonitoringTrafficStatistics'),
        'wlan':          ('WLAN status.', 'apiWlanBasicSettings'),
        'wlan-hosts':    ('Connected hosts status.', 'apiWlanHostList'),
        'wlan-features': ('WLAN features.', 'apiWlanFeature'),
        'inbox':         ('SMS Inbox.', 'apiSmsInbox'),
        'module-switch': ('Global module switch.', 'apiGlobalModuleSwitch'),
        'sms-config':    ('SMS config.', 'apiSmsConfig'),
        'global-config': ('Global config.', 'apiGlobalConfigXml'),
        'device-config': ('Device config.', 'apiDeviceConfigXml'),
        'mac-filter':    ('MAC Filter.', 'apiShowMacFilter'),
        }

    for k in showarr:
        prsr = showSubParsers.add_parser(k, help = showarr[k][0])
        prsr.add_argument('fields', nargs = '*', type = str, metavar = 'FILTER', help = 'XML XPath spec.')

    # Config 'data'
    dataSubParsers = data_parser.add_subparsers(title='item', dest='item', help = 'Available items')
    dataarr = {
        'on':           ('Mobile data on.', True),
        'off':          ('Mobile data off.', False),
        }

    for k in dataarr:
        prsr = dataSubParsers.add_parser(k, help = dataarr[k][0])
        prsr.add_argument('fields', nargs = '*', type = str, metavar = 'FILTER', help = 'XML XPath spec.')
 
    # Config 'macfilter'
    filter_parser.add_argument('type', type = str, help = 'disable, allow, deny')
    filter_parser.add_argument('mac', nargs = '+', type = str, help = 'MAC address.')

    # Config 'sendsms'
    send_parser.add_argument('message', type = str, help = 'Message.')
    send_parser.add_argument('phone_number', nargs = '+', type = str, help = 'Recipient phone number.')

    args = parser.parse_args()

    if args.commands == None: parser.print_help()
    else:
        if args.s != None: 
            pwd = args.s
        else:
            if passwordSHA256 == None:
                raise ValueError('\nPassword hash variable is not set. Please edit \'{}\' and set variable \'passwordSHA256\' to match your router\'s password (sha256) or alternatively set password with command line option -s.\n'.format(__file__))
            else:
                pwd = passwordSHA256

        hw = Hwcli(baseUrl = args.t, password = pwd)
        hw.debug = args.debug
        hw.login()

        if args.commands == 'show':
            try:
                getattr(hw, showarr[args.item][1])(args.fields)
            except KeyError as e:
                show_parser.print_help()
        elif args.commands == 'sendsms':
            hw.apiSendSms(message = args.message, recipients = args.phone_number)
        elif args.commands == 'reboot':
            hw.apiReboot()
        elif args.commands == 'data':
            hw.apiDataSwitch(dataarr[args.item][1])
        elif args.commands == "mac-filter":
            try:
                hw.apiSetMacFilter(args.mac, Hwcli.mac_filter_types[args.type])
            except KeyError as e:
                show_parser.print_help()

if __name__ == '__main__':
    try:
        parseArgumentsAndRun()
    except Exception as e:
        print(str(e))
