#include "signverifymessagedialog.h"
#include "ui_signverifymessagedialog.h"

#include "addressbookpage.h"
#include "base58.h"
#include "guiutil.h"
#include "init.h"
#include "main.h"
#include "optionsmodel.h"
#include "walletmodel.h"
#include "wallet.h"

#include <QClipboard>

#include <string>
#include <vector>

SignVerifyMessageDialog::SignVerifyMessageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignVerifyMessageDialog),
    model(0)
{
    ui->setupUi(this);

#if (QT_VERSION >= 0x040700)
    /* Do not move this to the XML file, Qt before 4.7 will choke on it */
    ui->addressIn_SM->setPlaceholderText(tr("Enter a MaxCoin address (e.g. mhN8btKtp3HrXgZJwyBakUzALLZ34nA4J)"));
    ui->pubkeyOut_SM->setPlaceholderText(tr("Click \"Sign Message\" to view public key"));
    ui->signatureOut_SM->setPlaceholderText(tr("Click \"Sign Message\" to generate signature"));

    ui->pubkeyIn_VM->setPlaceholderText(tr("Enter the public key used for signing"));
    ui->signatureIn_VM->setPlaceholderText(tr("Enter MaxCoin signature"));
#endif

    GUIUtil::setupAddressWidget(ui->addressIn_SM, this);

    ui->addressIn_SM->installEventFilter(this);
    ui->messageIn_SM->installEventFilter(this);
    ui->signatureOut_SM->installEventFilter(this);
    ui->messageIn_VM->installEventFilter(this);
    ui->signatureIn_VM->installEventFilter(this);

    ui->pubkeyOut_SM->setFont(GUIUtil::bitcoinAddressFont());
    ui->pubkeyIn_VM->setFont(GUIUtil::bitcoinAddressFont());
    ui->signatureOut_SM->setFont(GUIUtil::bitcoinAddressFont());
    ui->signatureIn_VM->setFont(GUIUtil::bitcoinAddressFont());
}

SignVerifyMessageDialog::~SignVerifyMessageDialog()
{
    delete ui;
}

void SignVerifyMessageDialog::setModel(WalletModel *model)
{
    this->model = model;
}

void SignVerifyMessageDialog::setAddress_SM(const QString &address)
{
    ui->addressIn_SM->setText(address);
    ui->messageIn_SM->setFocus();
}

void SignVerifyMessageDialog::showTab_SM(bool fShow)
{
    ui->tabWidget->setCurrentIndex(0);

    if (fShow)
        this->show();
}

void SignVerifyMessageDialog::showTab_VM(bool fShow)
{
    ui->tabWidget->setCurrentIndex(1);
    if (fShow)
        this->show();
}

void SignVerifyMessageDialog::on_addressBookButton_SM_clicked()
{
    if (model && model->getAddressTableModel())
    {
        AddressBookPage dlg(AddressBookPage::ForSending, AddressBookPage::ReceivingTab, this);
        dlg.setModel(model->getAddressTableModel());
        if (dlg.exec())
        {
            setAddress_SM(dlg.getReturnValue());
        }
    }
}

void SignVerifyMessageDialog::on_pasteButton_SM_clicked()
{
    setAddress_SM(QApplication::clipboard()->text());
}

void SignVerifyMessageDialog::on_signMessageButton_SM_clicked()
{
    /* Clear old signature to ensure users don't get confused on error with an old signature displayed */
    ui->signatureOut_SM->clear();

    CBitcoinAddress addr(ui->addressIn_SM->text().toStdString());
    if (!addr.IsValid())
    {
        ui->addressIn_SM->setValid(false);
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(tr("The entered address is invalid.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }
    CKeyID keyID;
    if (!addr.GetKeyID(keyID))
    {
        ui->addressIn_SM->setValid(false);
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(tr("The entered address does not refer to a key.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }

    WalletModel::UnlockContext ctx(model->requestUnlock());
    if (!ctx.isValid())
    {
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(tr("Wallet unlock was cancelled."));
        return;
    }

    CKey key;
    if (!pwalletMain->GetKey(keyID, key))
    {
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(tr("Private key for the entered address is not available."));
        return;
    }

    // get the public key from UI
    std::vector<unsigned char> vchPubKey = key.GetPubKey().Raw();

    CDataStream ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << ui->messageIn_SM->document()->toPlainText().toStdString();

    std::vector<unsigned char> vchSig;
    if (!key.Sign(HashKeccak(ss.begin(), ss.end()), vchSig))
    {
        ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_SM->setText(QString("<nobr>") + tr("Message signing failed.") + QString("</nobr>"));
        return;
    }

    ui->statusLabel_SM->setStyleSheet("QLabel { color: green; }");
    ui->statusLabel_SM->setText(QString("<nobr>") + tr("Message signed.") + QString("</nobr>"));

    ui->pubkeyOut_SM->setText(QString::fromStdString(HexStr(vchPubKey)));
    ui->signatureOut_SM->setText(QString::fromStdString(HexStr(vchSig)));
}

void SignVerifyMessageDialog::on_copySignatureButton_SM_clicked()
{
    QApplication::clipboard()->setText(ui->signatureOut_SM->text());
}


void SignVerifyMessageDialog::on_clearButton_SM_clicked()
{
    ui->addressIn_SM->clear();
    ui->messageIn_SM->clear();
    ui->signatureOut_SM->clear();
    ui->statusLabel_SM->clear();

    ui->addressIn_SM->setFocus();
}

void SignVerifyMessageDialog::on_verifyMessageButton_VM_clicked()
{
    std::vector<unsigned char> vchSig = ParseHex(ui->signatureIn_VM->text().toStdString());
    CDataStream ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << ui->messageIn_VM->document()->toPlainText().toStdString();

    // get the public key from Ui
    std::vector<unsigned char> vchPubKey = ParseHex(ui->pubkeyIn_VM->text().toStdString());
    
    CPubKey pubkey(vchPubKey);
    if (!pubkey.IsValid())
    {
        ui->pubkeyIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The public key is not valid.") + QString(" ") + tr("Please check it and try again."));
        return;    
    }

    CKey key;
    if (!key.SetPubKey(pubkey))
    {
        ui->pubkeyIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The public key cannot be added.") + QString(" ") + tr("Please check it and try again."));
        return;
    }

    if (!key.Verify(HashKeccak(ss.begin(), ss.end()), vchSig))
    {
        ui->signatureIn_VM->setValid(false);
        ui->statusLabel_VM->setStyleSheet("QLabel { color: red; }");
        ui->statusLabel_VM->setText(tr("The signature did not match the message digest.") + QString(" ") + tr("Please check the signature and try again."));
        return;
    }

    ui->statusLabel_VM->setStyleSheet("QLabel { color: green; }");
    ui->statusLabel_VM->setText(QString("<nobr>") + tr("Message verified.") + QString("</nobr>"));
}

void SignVerifyMessageDialog::on_clearButton_VM_clicked()
{
    ui->signatureIn_VM->clear();
    ui->messageIn_VM->clear();
    ui->statusLabel_VM->clear();
}

bool SignVerifyMessageDialog::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::FocusIn)
    {
        if (ui->tabWidget->currentIndex() == 0)
        {
            /* Clear status message on focus change */
            ui->statusLabel_SM->clear();

            /* Select generated signature */
            if (object == ui->signatureOut_SM)
            {
                ui->signatureOut_SM->selectAll();
                return true;
            }
        }
        else if (ui->tabWidget->currentIndex() == 1)
        {
            /* Clear status message on focus change */
            ui->statusLabel_VM->clear();
        }
    }
    return QDialog::eventFilter(object, event);
}
