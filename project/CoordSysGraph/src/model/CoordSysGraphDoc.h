// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// CoordSysGraphDoc.h : interface of the CCoordSysGraphDoc class
//


#pragma once


class CCoordSysGraphDoc : public CDocument
{
protected: // create from serialization only
	CCoordSysGraphDoc() noexcept;
	DECLARE_DYNCREATE(CCoordSysGraphDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	BOOL OnNewDocument() override;
	void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDLERS
	void InitializeSearchContent() override;
	void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) override;
#endif // SHARED_HANDLERS

// Implementation
public:
	~CCoordSysGraphDoc() override;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
