#pragma once

// 매크로 등 기능 정리
// #define 기능을 사용 시 본래 한 줄만 담을 수 있음
// define에 담고자 하는 여러 줄을 \로 묶어줄 수 있음
//	ㄴ 마지막 줄은 \를 붙히지 않고 사용
// define 주의사항 : 남발 시 debug에서 잡히지 않을 수 있음,

// 싱글톤 생성
#define DECLARE_SINGLE(classname)			\
private:									\
	classname() {}							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

// 싱글턴 호출
#define GET_SINGLE(classname)	classname::GetInstance()

// 안전한 제거
#define SAFE_DELETE(ptr)		\
	if (ptr)					\
	{							\
		delete ptr;				\
		ptr = nullptr;			\
	}