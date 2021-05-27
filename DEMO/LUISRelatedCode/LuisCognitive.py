# To run this sample, install the following modules.
# pip install azure-cognitiveservices-language-luis

# <Dependencies>
from azure.cognitiveservices.language.luis.authoring import LUISAuthoringClient
from azure.cognitiveservices.language.luis.runtime import LUISRuntimeClient
from msrest.authentication import CognitiveServicesCredentials
from functools import reduce

import json, time, requests, os

print(os.getcwd())
# </Dependencies>
class Luis:
    authoringKey = None
    authoringEndpoint = None
    predictionKey = None
    predictionEndpoint = None

    appName = None
    versionId = "0.1"
    intentName = None
    client = None

    app_id = None
    clientRuntime = None
    responseEndpointInfo = None
    fileName = None
    
    utterance = None    
    headers = {
        }
    params ={
            'query': utterance,
            'timezoneOffset': '0',
            'verbose': 'true',
            'show-all-intents': 'true',
            'spellCheck': 'false',
            'staging': 'false',
            'subscription-key': predictionKey 
        }    
    
    def begin_play(self):
        self.owner = self.uobject.get_owner()
        self.authoringKey = self.owner.get_property('AuthoringKey')
        self.authoringEndpoint = self.owner.get_property('AuthoringEndpoint')
        self.predictionKey = self.owner.get_property('PredictionKey')
        self.predictionEndpoint = self.owner.get_property('PredictionEndpoint')
        self.app_id = self.owner.get_property('AppId')

        enumLanguage = self.owner.get_property('TargetLanguage')
        self.target_language = self.owner.call_function('GetStringEnumLanguage', enumLanguage)
        if not self.authoringKey : return
        self.client = LUISAuthoringClient(self.authoringEndpoint, CognitiveServicesCredentials(self.authoringKey))        
  
    def set_luis(self):
        self.owner = self.uobject.get_owner()
        self.authoringKey = self.owner.get_property('AuthoringKey')
        self.authoringEndpoint = self.owner.get_property('AuthoringEndpoint')
        self.predictionKey = self.owner.get_property('PredictionKey')
        self.predictionEndpoint = self.owner.get_property('PredictionEndpoint')
        self.app_id = self.owner.get_property('AppId')

        enumLanguage = self.owner.get_property('TargetLanguage')
        self.target_language = self.owner.call_function('GetStringEnumLanguage', enumLanguage)

        self.client = LUISAuthoringClient(self.authoringEndpoint, CognitiveServicesCredentials(self.authoringKey))
        
    def create_app(self):
        # <AuthoringCreateApplication>
        # define app basics        
        appDefinition = {
            "name": self.appName,
            "initial_version_id": self.versionId,
            "culture": self.target_language
        }        
        # create app
        self.app_id = self.client.apps.add(appDefinition)

        # get app id - necessary for all other changes
        print("Created LUIS app with ID {}".format(self.app_id))
        # </AuthoringCreateApplication>
        self.owner.set_property('AppId', self.app_id)

    # </createApp>

     # <Add intent>
    def add_intent(self):
        self.intentName = self.owner.get_property('IntentName')                        
        self.client.model.add_intent(self.app_id, self.versionId, self.intentName)        
    # </Add intent>    
    
        
    def add_entities(self):                
        # <AuthoringAddEntities>
        # Add Prebuilt entity        
        print(os.getcwd())
        print(self.fileName)
        
        self.fileName = self.owner.get_property('FileName')                        
        with open(os.path.realpath(self.fileName),'r')as json_file:
            mlEntityDefinition = json.load(json_file)                               
        
        # add entity to app
        modelId = self.client.model.add_entity(self.app_id, self.versionId, name= self.fileName.split('.')[0], children=mlEntityDefinition)
        # </AuthoringAddEntities>    	
   
    # <AuthoringSortModelObject>
    def from_luis(self):
        try:
            self.owner = self.uobject.get_owner()
            self.utterance = self.uobject.get_owner().get_property('Utterance')
            print(self.utterance)
            self.headers = {
            }
            self.params ={
                'query': self.utterance,
                'timezoneOffset': '0',
                'verbose': 'true',
                'show-all-intents': 'true',
                'spellCheck': 'false',
                'staging': 'false',
                'subscription-key': self.predictionKey
            }                   
            print(f'{self.predictionEndpoint}luis/prediction/v3.0/apps/{self.app_id}/slots/production/predict')
            response = requests.get(f'{self.predictionEndpoint}luis/prediction/v3.0/apps/{self.app_id}/slots/production/predict',headers = self.headers ,params = self.params)
            print(response.json())
            topIntent = response.json()['prediction']['topIntent']
            score = response.json()['prediction']['intents'][topIntent]['score']            
            print(response.json()['prediction']['topIntent'])
            print(response.json()['prediction']['intents'][topIntent]['score'])
            entitiesName = response.json()['prediction']['entities']
            entitiesList = list(entitiesName.keys())
            listValue = []
            realValue = []
            realrealValue = []
            for value in entitiesList:                
                if value == '$instance' : break
                listValue.append(response.json()['prediction']['entities'][value])
                print(response.json()['prediction']['entities'][value])
        

            #print(listValue)
                
            for value in listValue:
                realValue.append(list(value[0].keys()))
                #print(realValue)
                
            for value in realValue:
                i = 0                
                realrealValue.append(value[i])
                i+= 1
                #print(realrealValue)
                
            print(realrealValue)
            self.owner.set_property("IntentScore",score)
            self.owner.set_property("TopIntentName",topIntent)
            self.owner.set_property("EntitieList",realrealValue)
            self.owner.call_function('BroadcastFinished', realrealValue,topIntent,score, True)
            
        except Exception as e:
                # Display the error string.
                print(f'{e}') 
          
