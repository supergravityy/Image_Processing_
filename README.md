# C기반 디지털 영상처리
&nbsp;

&nbsp;

## 💪 프로젝트 소개
Digital Image Processing (3rd) 의 주요기능을 직접 C만을 이용하여 구현해보는 프로젝트입니다
&nbsp;

&nbsp;

## 😄 주요기능
* 반전 처리
* 파라볼라 처리
* 감마 처리
* 블러링 처리
* 엣지검출 처리
* 엠보싱 처리
* 중간값 정렬 처리
* 확대 처리
* 축소 처리
* 회전 처리
* 히스토그램 평활화 처리
* 히스토그램 스트래칭 처리
* 이진화 처리
&nbsp;

&nbsp;

## 😒 프로젝트의 고유 특징
* 필터를 직접 설계할 수 있다
* 시간복잡도를 줄이기 위해 선형대수의 개념을 이용한다
* 간단함을 위해 흑백이면서, BPP 를 8로 고정시켜놓았다. (그외의 파일은 오류로 처리)
&nbsp;

&nbsp;

## ℹ️ 개발 정보
* 개발기간 : 24.03.22 ~ 05.23
* 개발인원 : 류성수
* 개발환경 : Visual Studio (C)
&nbsp;

&nbsp;

## ⚡ 설치 및 실행방법
### 1.먼저 레포지토리를 clone 합니다
```sh
git clone https://github.com/supergravityy/Image_Processing_.git
```
### 2. 코드를 컴파일 하여, 해당 콘솔창으로 이동합니다
사용할 수 있는 사진파일들을 위에 출력해놓았습니다. 아무거나 하나를 사용하시면 됩니다. (꼭, .bmp 까지 작성할것)
![스크린샷 2024-06-25 172310](https://github.com/supergravityy/Digital_communication/assets/145382604/91057565-4306-4146-bb84-b9b4c84af175)

### 3. 원하는 이미지 처리를 고릅니다
``` 예시로 블러링 처리를 골랐습니다 (1) 출력파일의 이름은 '선택파일_blurred.bmp' 로 정해집니다 ```
&nbsp;
![스크린샷 2024-06-25 172334](https://github.com/supergravityy/Digital_communication/assets/145382604/44aea434-5238-47ed-93b8-d358d4dac5af)

### 4. 해당 모드에서 필요한 정보들을 입력합니다
``` 2차원 가우시안 필터를 골랐습니다 ```
&nbsp;

&nbsp;
![스크린샷 2024-06-25 172431](https://github.com/supergravityy/Digital_communication/assets/145382604/e4bfba63-d5aa-47cb-9d8e-4199de8ac0be)
``` 필터의 크기를 7*7로 선언하였습니다 ```
&nbsp;

&nbsp;
![스크린샷 2024-06-25 172443](https://github.com/supergravityy/Digital_communication/assets/145382604/fb6a8b68-637e-4571-be50-6457d9c1d716)
``` 표준편차값을 1로 선언하였습니 ```
&nbsp;

&nbsp;
![스크린샷 2024-06-25 172502](https://github.com/supergravityy/Digital_communication/assets/145382604/130c07a7-5304-4a1d-a98e-f073dc935a07)

### 5. 이미지처리가 끝날때까지 기다립니다
``` 처리된 파일은 복제된 레포지토리 내의 Image_Processing_ 파일에서 찾을 수 있습니다. ```
&nbsp;

&nbsp;
![스크린샷 2024-06-25 172516](https://github.com/supergravityy/Digital_communication/assets/145382604/e3d8f75b-607c-43ef-acd6-a7519002997f)

&nbsp;

&nbsp;
## 🤷‍♂️ 프로그램 동작
``` 원본 ```
&nbsp;

![baboon](https://github.com/supergravityy/Digital_communication/assets/145382604/b98b3332-f709-4089-ab38-21e0f7261850)

&nbsp;
``` 블러링 처리 ```
&nbsp;

![baboon_blurred](https://github.com/supergravityy/Digital_communication/assets/145382604/dfe7e474-b868-4c62-9ddd-ceaa108ef356)
