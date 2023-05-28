import { SearchRounded } from '@mui/icons-material'
import {
  Box,
  Button,
  Chip,
  Divider,
  FormControl,
  FormHelperText,
  FormLabel,
  Input,
  Sheet,
  Stack,
  Tab,
  TabList,
  TabPanel,
  Tabs,
  Typography,
  tabClasses,
} from '@mui/joy'
import React, { FC, useState } from 'react'
import AttachMoneyIcon from '@mui/icons-material/AttachMoney'
import ArrowDownwardIcon from '@mui/icons-material/ArrowDownward'
import ArrowUpwardIcon from '@mui/icons-material/ArrowUpward'
import { moneyDeal, saveMoney, withdrawMoney } from '../utils/datafetch'

const TransferTab: FC = () => {
  const [toUserId, setUserId] = useState('')
  const [amount, setAmount] = useState('0')
  return (
    <Stack gap={2}>
      <Typography level='h2' fontSize={'lg'}>
        Transfer money
      </Typography>
      <Divider />
      <form
        onSubmit={(ev) => {
          ev.preventDefault()
          moneyDeal(Number(amount), toUserId)
            .then((res) => {
              if (res) {
                alert('transfer success')
                setAmount('0')
              } else {
                alert('transfer failed')
              }
            })
            .catch((err) => {
              alert(`transfer failed ${err}`)
            })
        }}
      >
        <Stack gap={1}>
          <FormControl required>
            <FormLabel>Id of the recipient</FormLabel>
            <Input
              placeholder='place id'
              value={toUserId}
              onChange={(ev) => {
                setUserId(ev.target.value)
              }}
            />
          </FormControl>
          <FormControl required>
            <FormLabel>amount</FormLabel>
            <Input
              startDecorator={'¥'}
              type='number'
              placeholder='amount to transfer'
              value={amount}
              onChange={(ev) => {
                setAmount(ev.target.value)
              }}
            />
          </FormControl>
          <Button type='submit'>Confirm!</Button>
        </Stack>
      </form>
    </Stack>
  )
}

const DepositTab: FC = () => {
  const [amount, setAmount] = useState('0')
  return (
    <Stack gap={2}>
      <Typography level='h2' fontSize={'lg'}>
        Deposit money
      </Typography>
      <Divider />
      <form
        onSubmit={(ev) => {
          ev.preventDefault()
          saveMoney(Number(amount))
            .then((res) => {
              if (res) {
                alert('deposit success')
                setAmount('0')
              } else {
                alert('deposit failed')
              }
            })
            .catch((err) => {
              alert(`deposit failed ${err}`)
            })
        }}
      >
        <Stack gap={1}>
          <FormControl required>
            <FormLabel>amount</FormLabel>
            <Input
              startDecorator={'¥'}
              type='number'
              placeholder='amount to deposit'
              value={amount}
              onChange={(ev) => {
                setAmount(ev.target.value)
              }}
            />
          </FormControl>
          <Button type='submit'>Confirm!</Button>
        </Stack>
      </form>
    </Stack>
  )
}

const WithdrawTab: FC = () => {
  const [amount, setAmount] = useState('0')

  return (
    <Stack gap={2}>
      <Typography level='h2' fontSize={'lg'}>
        Withdraw money
      </Typography>
      <Divider />
      <form
        onSubmit={(ev) => {
          ev.preventDefault()
          withdrawMoney(Number(amount))
            .then((res) => {
              if (res) {
                alert('withdraw success')
                setAmount('0')
              } else {
                alert('withdraw failed')
              }
            })
            .catch((err) => {
              alert(`withdraw failed ${err}`)
            })
        }}
      >
        <Stack gap={1}>
          <FormControl required>
            <FormLabel>amount</FormLabel>
            <Input
              startDecorator={'¥'}
              type='number'
              placeholder='amount to withdraw'
              value={amount}
              onChange={(ev) => {
                setAmount(ev.target.value)
              }}
            />
          </FormControl>
          <Button type='submit'>Confirm!</Button>
        </Stack>
      </form>
    </Stack>
  )
}

const OperationTabs: React.FC = () => {
  const [index, setIndex] = React.useState(0)
  return (
    <Sheet
      variant='outlined'
      sx={{
        bgcolor: 'background.body',
        flexGrow: 1,

        overflowX: 'hidden',
        borderRadius: 'md',
      }}
    >
      <Tabs
        aria-label='Pipeline'
        value={index}
        onChange={(event, value) => setIndex(value as number)}
        sx={{ '--Tabs-gap': '0px' }}
      >
        <TabList
          variant='plain'
          sx={{
            width: '100%',
            maxWidth: 400,
            mx: 'auto',
            pt: 2,
            alignSelf: 'flex-start',
            [`& .${tabClasses.root}`]: {
              bgcolor: 'transparent',
              boxShadow: 'none',
              '&:hover': {
                bgcolor: 'transparent',
              },
              [`&.${tabClasses.selected}`]: {
                color: 'primary.plainColor',
                fontWeight: 'lg',
                '&:before': {
                  content: '""',
                  display: 'block',
                  position: 'absolute',
                  zIndex: 1,
                  bottom: '-1px',
                  left: 'var(--ListItem-paddingLeft)',
                  right: 'var(--ListItem-paddingRight)',
                  height: '3px',
                  borderTopLeftRadius: '3px',
                  borderTopRightRadius: '3px',
                  bgcolor: 'primary.500',
                },
              },
            },
          }}
        >
          <Tab>
            <AttachMoneyIcon /> Transfer
          </Tab>
          <Tab>
            <ArrowDownwardIcon /> Deposit
          </Tab>
          <Tab>
            <ArrowUpwardIcon />
            {''}
            Withdraw
          </Tab>
        </TabList>

        <Box p={2}>
          <TabPanel value={0}>
            <TransferTab />
          </TabPanel>
          <TabPanel value={1}>
            <DepositTab />
          </TabPanel>
          <TabPanel value={2}>
            <WithdrawTab />
          </TabPanel>
        </Box>
      </Tabs>
    </Sheet>
  )
}

export const Home: FC = () => {
  return (
    <Stack direction={'column'} gap={2}>
      <Typography level='h1'>Home</Typography>

      <OperationTabs />
    </Stack>
  )
}
