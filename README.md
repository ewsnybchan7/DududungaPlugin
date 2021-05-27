# Dududunga Plugin
Integrated Azure Speech cognitive service into Unreal Engine 4

- [Dududunga Plugin](#dududunga-plugin)
  - [How it works](#how-it-works)
    - [Based on Unreal engine python](#based-on-unreal-engine-python)
    - [Azure cognitive service : Speech(stt)](#azure-cognitive-service-:-speech(stt))
    - [Azure cognitive service : LUIS](#azure-cognitive-service-:-luis)
  - [Installation](#installation)
    - [from Binaries](#from-binaries)
    - [from Sources](#from-sources)
  - [Getting Started](#getting-started)
    - [connect to Azure portal](#connect-to-azure-portal)      
***
# How it works
## Unreal Engine Python
## Azure Cognitive Services: Speech
## Azure cognitive service : LUIS
**python을 쓴 이유**

* Speech나 NLP같은 경우 머신러닝을 사용하게 된다. 머신러닝을 사용하게 될 경우 가장 많이 사용되는 언어는 python이므로 LUIS, Azure Speech의 Client사용에 있어 공통적으로 필요한 언어인 python을 사용하였다.

**UnrealEnginePython Plugin 과 Azure**

* UE에서도 python script를 지원하지만 Azure에서 사용하는 python의 설치를 위해서는 pip 21.1이상의 버전이 필요한데 UE에서는 20.1까지밖에 지원을 하지않아 라이브러리가 설치가 되지않는다.  UnrealEnginePython plugin을 이용하면 python과 UE를 연결할수 있고 이 plugin은 3.6버전의 python을 이용하기 때문에  Azure 라이브러리를 설치할 수 있어 사용하였다.

* python script를 살펴보면 설치한 `Azure 라이브러리`를 import 하여준다.  그 다음 UE에서 받아올 데이터들을 다 class의 변수로 저장을 한뒤 UEPython plugin을 이용하여 해당 변수와의 상호작용을 한다. script에서는 key, endpoint 그리고 발화문을 이용하게 되고 사용자는 블루프린트를 이용하여 해당 데이터을 넣거나 UE 디테일 창을 이용하여 해당하는 데이터를 넣게 된다. 
* 참조: [LUIS Docs]( https://docs.microsoft.com/ko-kr/azure/cognitive-services/luis/client-libraries-rest-api?tabs=windows&pivots=programming-language-python)


***
# Installation

***
# Getting Started
## Azure ID 연결

Azure cognitive services를 사용하기 위해서는 [Azure Portal](https://azure.microsoft.com/)에 로그인을 하고, `Speech` 리소스와 `LUIS` 리소스를 생성해야 합니다.
ㄷㄷㄷㅈ 플러그인에서 speech 서비스를 사용하기 위해 필요한 것은 다음과 같습니다.

 1. 구독키
 2. 엔드포인트 (Custom Model 사용시)

그리고 자연어처리 LUIS 서비스를 사용하기 위해 필요한 것은 다음과 같습니다.

 1.  병선이 루이스 키 작업
 2. 
 3. 

### Speech
AzureSpeechSDK에서 `Speech Recognition` C++ 클래스를 확인할 수 있습니다.
Speech 서비스를 사용하기 위해 해당 클래스 하나를 생성합니다. 생성한 Speech Recognition의 Property `Settings`에 구독키, 서버 지역, 타겟 언어를 설정합니다.

/////////////settings 사진//////////////

음성 인식을 시작하기 위한 준비는 다 끝났습니다!
음성 인식을 원하는 블루프린트에서 생성한 `Speech Recognition`을 찾고, 이벤트 처리를 해줍니다. 이벤트는 총 세 가지가 있습니다.

///// 델리게이트 이벤트 /////
블루프린트에서 호출이 가능하며, 해당 델리게이트에 커스텀 이벤트를 바인딩하여 이벤트 처리를 할 수 있습니다.
 1. Start 이벤트 **(필수)**
////// start 이벤트 연결 사진 /////
`Start` 이벤트는 음성 인식을 시작할 때 호출되는 이벤트입니다.
주로 이벤트를 호출하기 전에 음성 인식 중임을 사용자에게 알리기 위한 목적으로 UI, 오브젝트의 변화를 처리하면 됩니다.
현재는 필수적으로 Start 이벤트를 호출하면, 음성 인식을 시작할 수 있도록 `Run Speech Recognition`를 사용자 레벨에서 호출해야 합니다.

	**Run Speech Recognition**은 실질적으로 음성 인식을 하는 메서드로 어떤 음성 인식을 할 지 설정할 수 있습니다.
	- `Continuous`: **단일 인식**과 **연속 인식** 중 하나를 선택할 수 있습니다.
	- `Use Phrase List`: 인식률 향상 기능인 **구 리스트** 기능을 사용할 지 결정합니다.
	- `Use Custom Model`: 인식률 향상 기능인 **사용자 모델** 기능을 사용할 지 결정합니다.

 2. Recognizing 이벤트
//////// recognizing 이벤트 연결 사진
`Recognizing` 이벤트는 음성 인식 중에 `Run Speech Recognition`을 통해 음성 인식을 재시도할 때 호출되는 이벤트입니다.
런타임 도중, 사용자에게 음성 인식에 여러 번 접근하는 경우를 막기 위한 용도로 사용하면 됩니다.

 4. Finish 이벤트
/////// finish 이벤트 연결 사진
`Finish` 이벤트는 음성 인식이 끝났을 때 자동으로 호출되는 이벤트로 음성 인식의 결과값과 인식 성공 여부를 전달받을 수 있습니다.
개발자는 음성 인식이 끝났을 때 UI를 바꾸거나 오브젝트와 상호작용하는 등의 처리를 커스텀 이벤트로 만들고 바인딩하면 됩니다.

	**단일인식**은 Finish 이벤트가 한 번 호출되고, **연속인식**은 여러 번 호출됩니다.

이벤트 처리가 끝났다면 Azure의 음성 인식 서비스를 맘껏 즐기세요!

### LUIS


## Blueprint 설명
## 키보드 Blueprint 설명
키보드는 C++로 만들어진 함수들과 언리얼엔진 블루프린트에서 만든 함수들로 구성된다.  C++로 만들어진 `Combine Hangeul Auto Line Break` 함수를 숫자 자판, 영어(또는 한글 자판), 기타 기호 자판(ex. '-', '?')들이 사용하고 나머지 자판들은 다른 함수들을 사용하여 구성된다.
1. 숫자, 영어, 기타 기호 자판 구성
<img src=Basic_Keypad.jpg></img>
* 세부구성
  * <img src=Basic_Keypad_ZoomIn.jpg></img>
    * `Combine Hangeul Auto Line Break`는 함수로 들어온 `Text`의 값이 한글이 아니면 결과창에 `Text`의 값을 그대로 넣어준다.
    * `Text`의 값이 한글이라면 한글 결합 알고리즘인 한글 오토마타를 이용해 결합 후 결과창에 넣어준다.
2. 나머지 자판
<img src=Other_Keypad.jpg></img>
* Backspace
  * <img src=Backspace_ZoomIn.jpg></img>
    * Backspace 키보드 자판을 누르면 결과창에 있는 가상의 커서의 앞 부분의 Text가 지워진다.
    * 커서 앞 부분이 한글이고 커서가 이동하지 않았다면 한글 결합 알고리즘의 역순으로 분해된다.
    * 위의 경우가 아닌 경우에는 커서 앞 쪽의 한 개의 Text를 지운다.

* Enter
  * <img src=Enter_ZoomIn.jpg></img>
    * 결과창에는 자동 줄 바꿈이 적용되어 있다. Enter 키보드 자판을 누르게 되면 자동 줄 바꿈이 적용된 부분들이 없어진 String이 리턴되어 나온다.
* 방향키
  * <img src=ArrowKeys_ZoomIn.jpg></img>
    * 왼쪽, 오른쪽 방향키는 결과창에 있는 가상의 커서를 앞, 뒤로 1칸씩 이동시킨다.
    * 위 방향키는 줄 바꿈이 된 경우 아랫줄에서 윗줄로, 아래 방향키는 윗줄에서 아랫줄로 커서를 이동시킨다.
    * 커서가 첫 번째 줄의 6번째 Text의 뒤에 위치하고 있었으면 아래 방향키로 움직인 커서의 위치는 두 번째 줄의 6번째 Text 뒤에 위치하게 된다. 위 방향키는 아래 방향키의 반대다.
* ESC
  * <img src=ESC_ZoomIn.jpg></img>
    * ESC 키보드 자판을 누르면 `결과창의 Text`들을 다 지운다.

## 키보드와 STT 상호작용
<img src=Keyboard_UI.jpg></img>
키보드를 통해서 `STT`를 사용하기 위해서는 위의 사진에서 빨간색 사각형 안에 있는 하얀색 마이크 버튼을 누르면 된다. 음성인식이 끝나면 자동으로 음성인식이 종료되게 할 수도 있고, 버튼을 클릭해서 종료시킬 수도 있다. 키보드에서 STT를 사용하기 위한 절차를 설명하도록 한다.

1. 월드에 STT 생성 및 세팅
* C++ 클래스로 만들어져 있는 `SpeechRecognition`을 월드에 배치해둔다.
* <img src=Level_Blueprint_STT_BasicSettings.jpg></img>
* 레벨 블루프린트로 들어가 변수를 만든다. 변수 유형은 `Speech Recognition 오브젝트 레퍼런스`로 하고 변수 이름은 `Speech`로 지정했다
* 기본값은 월드에 배치한 `SpeechRecognition`을 찾아서 지정해준다. 위의 그림에서는 월드에 배치할 때 이름이 `SpeechRecognition1`으로 생성되었기 때문에 이를 기본값으로 해주었다.
* <img src=Level_Blueprint_STT_Blueprint.jpg></img>
* `BeginPlay 이벤트`를 만들어준 뒤 Ctrl을 누른 채로 `Speech`를 끌어 온다. 그 뒤 `STTSettings`라는 함수를 불러온 뒤 Region과 Language를 설정해준다.
* `Speech`를 클릭하면 이벤트를 설정해줄 수 있는데 여기서 `Started`를 눌러준다.  Ctrl을 누른 채로 `Speech`에서 `Run Speech Recognition` 함수를 불러와 연결해주면 준비가 끝난다.

2. 키보드에 STT 생성
<img src=Create_STT_ZoomIn.jpg></img>
* 키보드의 UI를 만든 위젯 블루프린트에서 `Construct 이벤트`에 `Get Actor of Class`를 연결한다.
*  `Get Actor of Class`의 `Actor Class`를 월드에 배치된 `SpeechRecognition`로 지정하고 변수로 승격시켜 주면 키보드에서 STT를 생성해 사용할 수 있다. 여기서도 레벨 블루프린트에서 사용했던 이름인 `Speech`로 변수 이름을 설정했다.

3. Mike 버튼 Blueprint
<img src=Call_STT.jpg></img>
`Keyboard_UI.jpg`에서 빨간색 사각형으로 강조되어 있는 하얀색 마이크 버튼을 누르면  발생하는 블루프린트 부분이다. 이 부분은 STT를 시작하는 부분과 STT가 연속 인식일 경우 마이크 버튼을 1번 더 누름으로써 종료시키는 부분으로 나뉜다.

* STT를 시작하는 부분
  * <img src=Start_STT_ZoomIn.jpg></img>
    * `On Mike`는 음성인식을 하는 중인지 아닌지를 판별하는 변수이다. 이 변수가 False이면 음성인식 중이 아니므로 Mike 버튼이 눌리면 음성인식을 시작하게 된다.
    * Mike 버튼의 이미지를 음성인식 중인 것을 잘 알 수 있도록 바꿔주고 `On Mike`를 True로 바꿔준다.
    * `Speech`를 Ctrl을 누른 채로 가져와서 놓은 뒤 `Started 호출`을 불러오면 음성인식을 사용할 수 있다.

* 연속인식 시 종료시키는 부분
  * <img src=Continuous_STT_End_ZoomIn.jpg></img>
    * `On Mike`가 True일 경우 음성인식 중이다. 음성인식 중에 Mike 버튼을 1번 더 눌러서 끄는 경우는 연속인식일 때만 해당되고 단일인식은 해당하지 않는다.
    * 연속인식을 판단하는 것은 `Speech`에서 `Get Is Continuous`로 확인한다. True이면 연속인식, False이면 단일인식이다.
    * `Get Is Continuous`가 True로 연속인식 중이라면 `On Mike`는 False로 만들어 음성인식을 종료시킨다.
    * Mike 버튼의 이미지를 기본 이미지로 바꿔준 뒤 `Speech`의 `Stop Speech Recognition`을 호출해주면 종료된다.
4. STT 종료될 때 자동 호출되는 함수 선언부
<img src=STT_End_ZoomIn.jpg></img>
* STT가 종료될 때 자동으로 호출되는 함수를 선언하려면 `Construct 이벤트`에서 Ctrl을 눌러 가져온 `Speech`에 `Finished에 이벤트 바인딩`을 하고 이와 연결된 커스텀 이벤트를 만들어줘야 한다. 
* 음성인식이 종료되고 자동으로 이 커스텀 이벤트가 실행되는데 이 커스텀 이벤트는 연속인식과 단일인식일 경우 2가지로 나뉜다. 각 경우를 알아본다
  * 연속인식일 경우
    * 연속인식은 `Speech`에서 불러온 `Get Is Continuous`의 값이 True일 경우이다.
    * 키보드 위젯 블루프린트의 부모 클래스인 `MultiLine_CppKoreanKeyboard`에 정의되어 있는 `Append String`을 이용해서 음성인식되는 String을 결과창에 계속 넣어준다.
  * 단일인식일 경우
    * 단일인식은 `Speech`에서 불러온 `Get Is Continuous`의 값이 False일 경우이다.  
    * `Append String`을 이용해 음성인식 된 String을 결과창에 넣어준다.
    * `On Mike`를 False로 바꿔주어 음성인식이 끝났

## LUIS 사용하기
UE4블루프린트를 이용하여 Luis사용하기
월드 아웃라이너에서 `Luis Actor`를 추가한다.
Luis 블루프린트의 `Authoring key`, `Prediction Key`, `Authoring Endpoint`,` prediction Endpoint`, `AppId`, `TargetLanguage`를 
블루프린트에서 플레이를 하기 전 `Luis`의 `Property`위치에서 해당 언어를 선택 할 수 있다.

<img src=luisProperty.PNG></img>

블루프린트로 들어와 LUIS 변수를 추가한다.

 <img src=변수추가.PNG></img>

  추가한 변수 유형을 `Luis`로 바꾸고 변수의 기본값을 월드 아웃라이너에 추가한 `Luis Actor` 바꾸어 준다.

* <img src=변수 기본값 설정.PNG></img>

	변수를 추가에 성공했다면 해당 `Luis Actor`의 블루프린트 노드를 사용할 수 있다.

* **Azure App 생성하기**

  *	`Luis`변수를 추가하면 `LuisCreateApp`노드를 사용할 수 있다. 해당 노드를 사용하고 나서 항상 `RunLuisCognition`을 사용하여 마무리를 해야한다.

  *	해당 노드들은 함수 변수에 추가하여 `async task`를 이용하여 `Background`에서 해당 함수를 구동하기 때문에 `RunLuisCognition`을 이용하여 함수 배열을 처리한다.

  *	블루프린트 노드를 사용하고 난 후 `RunLuisCognition`으로 닫아줘야한다.

	* <img src=luisCreateApp.PNG></img>

  		* 해당 노드를 추가하고 `UE`를 플레이하면  `탈`에 해당` LuisApp`이 추가된걸 확인할 수 있다.
* <img src=luisPortalMakeApp.PNG></img>


* **Intent 와 Entities 추가하기**

  * <img src=entities upload.PNG></img>


    *  `Intent`를 추가하는 방법은 해당 블루프린트 노드를 추가하고 노드에 FString 값을 넣어 해당 값으로` Intent`를 추가한다.

  * <img src=IntentName.PNG></img>

    *	`Luis 포탈`에서 `Intent`가 추가된 것을 확인할 수 있다.

  * <img src=UE4AppIntent.PNG></img>

    *	`Entities`는 `json`파일을 이용하여 해당 `Entities`를 추가할 수 있다.

    *	Luis의 `Entities`는 기본적으로 `json`형식으로 이루어져 있다. 따라서  `json` 파일로 해당 `Entities`를 추가한다.

  * <img src=EntitiesName.PNG></img>

    * `C:\Program Files\Epic Games\UE_4.25\Engine\Binaries\Win64` 위치에 해당`json`파일을 넣게되면 `포탈`에 `Entities`가 추가된다.



* **Luis train and publish하기**

  * `Luis 포탈`에서 우선 리소스를 할당 해야한다. 해당 프로젝트에서는 `UE4`에서 리소스를 할당 할 수 없다. 따라서 직접 `포탈`로 가서 할당을 해주어야한다.

  * `UE4`에서 `train`을 하게 될 경우 추가적인 시간이 걸리게 되는데 해당 시간은 플레이에 방해가 될 수 있으므로 추가하지 않았다. 따라서 `포탈`을 이용하여 `train`을 해야 한다.

  * <img src=portal.PNG></img>

    * 해당 `포탈`에서 `publish`도 같이 할 수 있다. `publish`를 하면  사용자가 블루프린트에서 발화문에 대한 점수와 결과를 `LuisUttranceScore`를 이용하여 사용할 수 있게 된다.

  * <img src=trainutterances.PNG></img>

    * 해당 사진처럼 `intent`를 학습시키고 `intent`에 있는 `entity`를 지정하여 문장의 의도를 명확히 한다.

  * <img src=publish.PNG></img>

    * `publish`를 하게 되면 해당 발화문과 비슷한 유형의 다른 발화문을 의도에 맞게 사용할 수 있게된다. `publish`를 하지 않으면 추가한 `train`부분을 사용할 수 없다.

* **Use Scoring and TopIntent**

  * <img src=ScoringBlueprint.PNG></img>

    * 사진의 블루프린트 노드를 이용하여 문장의 `TopIntent`와 `Entity Set`에 대한 정보를 다음과 같이 볼 수 있다.

  * <img src=UseScoring.PNG></img>

    * 다음과 같이 노드를 사용한 이후 `Finished` 이벤트를 이용하여 해당 노드의 작업이 끝난 이후 데이터를 가져올 수 있다.

  * <img src=pythonLog.PNG></img>

    * `Finished`를 이용하여 가져온 데이터를 확인할 수 있다.


## LUIS Intent 확인방법

 `UE` `출력Log창`을 띄어두면 해당 로그를 `Log python`를 통하여 확인할 수 있다.  주의 해야할 점은 `Entities`의 2번째 의도에 대해 접근을 하기 때문에 2번째의 의도를 이용하여 UE에서 사용을 해야한다. 만약 `Entity`의 이름이 `Pizza`이고 `Pizza`안에 `Size`, `Price`등을 넣게되면 사용하는 블루프린트는 `Pizza`의 `Size`나 `Price`에 접근하게 된다. `Size`안에 `Small` ,` Medium`, `Large`를 넣게 되어도 인식을 할 수 가 없다.
* <img src=pythonLog.PNG></img>
***
# Resources

***
# Release and Contributing

***
# License
