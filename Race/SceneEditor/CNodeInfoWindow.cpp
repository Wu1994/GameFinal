#include "stdafx.h"
#include "CNodeInfoWindow.h"
#include "EditorScene.h"
#include "ControlIDs.h"

void CNodeInfoWindow::OnCreate(HWND parent, int xPos, int yPos, int width, int height)
{
	CSubWindow::OnCreate(parent, xPos, yPos, width, height);

	HINSTANCE hInst = GetModuleHandle(NULL);

	HWND label1 = CreateLabel(IDC_NODE_INFO_LABEL, "T:", xPos - 10, 10, 20);
	HWND label2 = CreateLabel(IDC_NODE_INFO_LABEL + 1, "R:", xPos - 10, 40, 20);
	HWND label3 = CreateLabel(IDC_NODE_INFO_LABEL + 2, "S:", xPos - 10, 70, 20);
	
	const int textFieldWidth = 90;
	const int intervalBetweenTextFields = 2;

	for (u32 i = 0; i < 3; i++)
	{
		mPosTextFields[i] = CreateWindow(TEXT("edit"), NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER,
			xPos + 10 + i * (textFieldWidth + intervalBetweenTextFields), yPos + 10, textFieldWidth, 20, 
			mParentHwnd, (HMENU)(IDC_POS_TEXTFIELD + i), hInst, NULL);

		mRotTextFields[i] = CreateWindow(TEXT("edit"), NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER,
			xPos + 10 + i * (textFieldWidth + intervalBetweenTextFields), yPos + 40, textFieldWidth, 20,
			mParentHwnd, (HMENU)(IDC_ROT_TEXTFIELD + i), hInst, NULL);

		mScaleTextFields[i] = CreateWindow(TEXT("edit"), NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER,
			xPos + 10 + i * (textFieldWidth + intervalBetweenTextFields), yPos + 70, textFieldWidth, 20,
			mParentHwnd, (HMENU)(IDC_SCALE_TEXTFIELD + i), hInst, NULL);
	}

	mUpdateTransformBtn = CreateWindow(TEXT("button"), TEXT("Update"),
		WS_CHILD | WS_VISIBLE, xPos, yPos + 100, 100, 24,
		mParentHwnd, (HMENU)(IDC_UPDATE_TRANSFORM_BTN), hInst, NULL);

	mCancelTransformBtn = CreateWindow(TEXT("button"), TEXT("Cancel"), 
		WS_CHILD | WS_VISIBLE, xPos + 105, yPos + 100, 100, 24,
		mParentHwnd, (HMENU)(IDC_CANCEL_TRANSFORM_BTN), hInst, NULL);

	mStaticCheckBox = CreateWindow(TEXT("button"), TEXT("Static"), 
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		xPos, yPos + 130, 60, 20, mParentHwnd, (HMENU)IDC_NODE_INFO_STATIC_CHECKBOX, hInst, NULL);

	mPhysicsCheckBox = CreateWindow(TEXT("button"), TEXT("Bounding Physics"),
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		xPos + 70, yPos + 130, 150, 20, mParentHwnd, (HMENU)IDC_NODE_INFO_PHYSICS_CHECKBOX, hInst, NULL);

	mShadowCheckBox = CreateWindow(TEXT("button"), TEXT("Shadow"),
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		xPos, yPos + 150, 70, 30, mParentHwnd, (HMENU)IDC_NODE_INFO_SHADOW_CHECKBOX, hInst, NULL);
}

void CNodeInfoWindow::UpdateShowing(SNodeInfo* info)
{
	char text[256];

	if (info)
	{
		sprintf_s(text, "%f", info->Position.x);
		SetWindowTextA(mPosTextFields[0], text);

		sprintf_s(text, "%f", info->Position.y);
		SetWindowTextA(mPosTextFields[1], text);
		
		sprintf_s(text, "%f", info->Position.z);
		SetWindowTextA(mPosTextFields[2], text);

		sprintf_s(text, "%f", info->Rotation.x * 180.0f / XM_PI);
		SetWindowTextA(mRotTextFields[0], text);

		sprintf_s(text, "%f", info->Rotation.y * 180.0f / XM_PI);
		SetWindowTextA(mRotTextFields[1], text);

		sprintf_s(text, "%f", info->Rotation.z * 180.0f / XM_PI);
		SetWindowTextA(mRotTextFields[2], text);
		
		sprintf_s(text, "%f", info->Scaling.x);
		SetWindowTextA(mScaleTextFields[0], text);

		sprintf_s(text, "%f", info->Scaling.y);
		SetWindowTextA(mScaleTextFields[1], text);

		sprintf_s(text, "%f", info->Scaling.z);
		SetWindowTextA(mScaleTextFields[2], text);

		if (info->Static)
			SendMessage(mStaticCheckBox, BM_SETCHECK, BST_CHECKED, 0);
		else
			SendMessage(mStaticCheckBox, BM_SETCHECK, BST_UNCHECKED, 0);

		if (info->BoundingPhysics)
			SendMessage(mPhysicsCheckBox, BM_SETCHECK, BST_CHECKED, 0);
		else
			SendMessage(mPhysicsCheckBox, BM_SETCHECK, BST_UNCHECKED, 0);

		if (info->ShadowCasting)
			SendMessage(mShadowCheckBox, BM_SETCHECK, BST_CHECKED, 0);
		else
			SendMessage(mShadowCheckBox, BM_SETCHECK, BST_UNCHECKED, 0);
	}
}

void CNodeInfoWindow::OnCommand(WORD id, WORD event, WPARAM wParam, LPARAM lParam)
{
	if (id == IDC_UPDATE_TRANSFORM_BTN && event == BN_CLICKED)
	{
		OnClickUpdateTransformButton();
	}
	else if (id == IDC_CANCEL_TRANSFORM_BTN && event == BN_CLICKED)
	{
		OnClickCancelTransformButton();
	}
	else if (id == IDC_NODE_INFO_STATIC_CHECKBOX && event == BN_CLICKED)
	{
		OnClickStaticCheckBox();
	}
	else if (id == IDC_NODE_INFO_PHYSICS_CHECKBOX && event == BN_CLICKED)
	{
		OnClickPhysicsCheckBox();
	}
	else if (id == IDC_NODE_INFO_SHADOW_CHECKBOX && event == BN_CLICKED)
	{
		OnClickShadowCheckBox();
	}
}

void CNodeInfoWindow::OnClickUpdateTransformButton()
{
	EditorScene* scene = EditorScene::getInstance();
	SNodeInfo* info = scene->GetSelectedNodeInfo();
	if (!info)
		return;

	f32 position[3];
	f32 rotation[3];
	f32 scaling[3];

	char text[256];
	for (u32 i = 0; i < 3; i++)
	{
		GetWindowTextA(mPosTextFields[i], text, 256);
		int err = sscanf_s(text, "%f", &position[i]);
		if (err < 1 || err == EOF)
		{
			MessageBoxA(mParentHwnd, "cannot convert position value", "Error", NULL);
			return;
		}
	}

	for (u32 i = 0; i < 3; i++)
	{
		GetWindowTextA(mRotTextFields[i], text, 256);
		int err = sscanf_s(text, "%f", &rotation[i]);
		if (err < 1 || err == EOF)
		{
			MessageBoxA(mParentHwnd, "cannot convert rotation value", "Error", NULL);
			return;
		}
	}

	for (u32 i = 0; i < 3; i++)
	{
		GetWindowTextA(mScaleTextFields[i], text, 256);
		int err = sscanf_s(text, "%f", &scaling[i]);
		if (err < 1 || err == EOF)
		{
			MessageBoxA(mParentHwnd, "cannot convert scale value", "Error", NULL);
			return;
		}
	}

	info->Position.x = position[0];
	info->Position.y = position[1];
	info->Position.z = position[2];

	info->Rotation.x = rotation[0] * XM_PI / 180.0f;
	info->Rotation.y = rotation[1] * XM_PI / 180.0f;
	info->Rotation.z = rotation[2] * XM_PI / 180.0f;

	info->Scaling.x = scaling[0];
	info->Scaling.y = scaling[1];
	info->Scaling.z = scaling[2];

	scene->UpdateNodeInfo(info);
	UpdateShowing(info);
}

void CNodeInfoWindow::OnClickCancelTransformButton()
{
	EditorScene* scene = EditorScene::getInstance();
	SNodeInfo* info = scene->GetSelectedNodeInfo();
	if (!info)
		return;

	UpdateShowing(info);
}

void CNodeInfoWindow::OnClickStaticCheckBox()
{
	EditorScene* scene = EditorScene::getInstance();
	SNodeInfo* info = scene->GetSelectedNodeInfo();
	if (!info)
		return;

	LRESULT r = SendMessage(mStaticCheckBox, BM_GETCHECK, 0, 0);
	if (r == BST_CHECKED)
	{
		info->Static = true;
	}
	else if (r == BST_UNCHECKED) 
	{
		info->Static = false;
	}
}

void CNodeInfoWindow::OnClickPhysicsCheckBox()
{
	EditorScene* scene = EditorScene::getInstance();
	SNodeInfo* info = scene->GetSelectedNodeInfo();
	if (!info)
		return;

	LRESULT r = SendMessage(mPhysicsCheckBox, BM_GETCHECK, 0, 0);
	if (r == BST_CHECKED)
	{
		info->BoundingPhysics = true;
	}
	else if (r == BST_UNCHECKED)
	{
		info->BoundingPhysics = false;
	}
}

void CNodeInfoWindow::OnClickShadowCheckBox()
{
	EditorScene* scene = EditorScene::getInstance();
	SNodeInfo* info = scene->GetSelectedNodeInfo();
	if (!info)
		return;

	LRESULT r = SendMessage(mShadowCheckBox, BM_GETCHECK, 0, 0);
	if (r == BST_CHECKED)
	{
		info->ShadowCasting = true;
	}
	else if (r == BST_UNCHECKED)
	{
		info->ShadowCasting = false;
	}

	scene->UpdateNodeInfo(info);
}

